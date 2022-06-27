#include "main.h"
#include <getopt.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <signal.h>
#include <thread>
#include "RFDaemonServer.h"
#include "AppManager.h"
#include <console.h>
#include <signal.h>

using namespace std;

int tcp_console_port = 5000;
uint16_t port = DEFAULT_TCP_PORT;
AppManager* appManager = NULL;
RFDaemonServer* srv = NULL;
thread srvRxThread;
thread srvTxThread;

void interrupt_signal_handler(int signum);

int main(int argc, char* argv[])
{
    string configFileName;
    bool terminalMode = false, serverOnlyMode = false;
    pid_t daemonPid = 0;
    
    signal(SIGINT, interrupt_signal_handler);
    signal(SIGTERM, interrupt_signal_handler);
    
    if (checkRunArgs(argc, argv, port, configFileName, terminalMode))
    {
        configFileName = "applist.json";
        port = DEFAULT_TCP_PORT;
    }
    
    cout << "Starting in " << (terminalMode ? "terminal" : "daemon") << "mode.\n";

    if (!terminalMode)
        daemonPid = fork();
    if (daemonPid == -1) // main process code part
        cout << "Error: RFDaemon process fork failed.";
    else if (daemonPid == 0) // fork process code part
    {
        srv = new RFDaemonServer(port);
        appManager = new AppManager(configFileName);

        int appfile_error = appManager->loadConfigFile();
        if (appfile_error)
        {
            serverOnlyMode = true;
            cout << "Application script has errors. Server-only mode runned.\n";
        }
        
        if (!serverOnlyMode)
            appManager->runApps();

        start_tcp_console(tcp_console_port);
        srvRxThread = thread(tcpServerReceiveThreadHandler);
        srvTxThread = thread(tcpServerSendThreadHandler);
        if (terminalMode)
        {
            start_stdstream_console();
        }
        srvTxThread.join();
        srvRxThread.join();
    }
    return 0;
}

bool checkRunArgs(int argc, char* argv[], uint16_t& port, string& appListFileName, bool& terminalMode)
{
    uint16_t portVal = 0;
    size_t len = 0, portArgLen = 0;
    int opt = 0;
    bool wrongArg = false;
    char* portArgEnd = NULL;

    while ((opt = getopt(argc, argv, "tc:p:")) != -1)
    {
        switch (opt)
        {
        case 't':
            terminalMode = true;
            break;
        case 'd':
            terminalMode = false;
            break;
        case 'c':
            len = strlen(optarg);
            if (len > 5)
            {
                if (!strcmp(optarg + len - 5, ".json"))
                    appListFileName = string(optarg);
                else
                    wrongArg = true;
            }
            else
                wrongArg = true;
            break;
        case 'p':
            portArgLen = strlen(optarg);
            portArgEnd = optarg;
            portVal = (uint16_t)strtoul(optarg, &portArgEnd, 10);

            if ((size_t)(portArgEnd - optarg) < portArgLen)
            {
                portVal = (uint16_t)strtoul(optarg, &portArgEnd, 16);
                wrongArg = (size_t)(portArgEnd - optarg) < portArgLen;
                if (!wrongArg)
                    port = portVal;
            }
            else
                port = portVal;
            break;
        default:
            wrongArg = true;
        }
    }
    if (wrongArg || !len)
        printf("Usage: %s [-p 0..65535/0..FFFF] [-t] -c filename.json\n", argv[0]);
    return (wrongArg || !len);
}

int tcpServerSendThreadHandler()
{
    srv->setAppManager(appManager);
    return srv->sendThread();
}

int tcpServerReceiveThreadHandler()
{
    while (!srv)
        usleep(1000);
    return srv->receiveThread();
}

void exitHandler(int sig)
{
    delete srv;
    srv = nullptr;
    quick_exit(sig);
}

void interrupt_signal_handler(int sig)
{
    cout << "Interrupt signal received.\n";
    appManager->closeApps();
    exitHandler(sig);
}

// TODO 
void stop_all_threads()
{
}