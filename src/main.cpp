#include "main.h"
#include "AppManager.h"
#include "RFDaemonServer.h"
#include <console.h>
#include <getopt.h>
#include <iostream>
#include <signal.h>
#include <string.h>
#include <thread>
#include <unistd.h>

using namespace std;

bool VERBOSE = false;
bool WITHOUT_RFMEASK = false;

int tcp_console_port = 5000;
uint16_t port = DEFAULT_TCP_PORT;
AppManager *appManager = NULL;
RFDaemonServer *srv = NULL;
thread srvRxThread;
thread srvTxThread;

void interrupt_signal_handler(int signum);

int main(int argc, char *argv[])
{
    string configFileName;
    bool terminalMode = false, serverOnlyMode = false;
    pid_t daemonPid = 0;

    signal(SIGINT, interrupt_signal_handler);
    signal(SIGTERM, interrupt_signal_handler);

    checkRunArgs(argc, argv, port, configFileName, terminalMode);

    cout << "Starting in " << (terminalMode ? "terminal" : "daemon")
         << "mode.\n";

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

void print_help()
{
    nos::fprint(
        "Usage: rfdaemon [OPTION]...\n"
        "\n"
        "Configuration:\n"
        "  -c, --config - path to settings file\n"
        "  -p, --port - RFDaemon protocol port\n"
        "  -d, --daemon - Start daemon mode (default)\n"
        "  -t, --terminal - Start terminal mode \n"
        "  -n, --noext - Disable rfmeask extention \n"
        "Debug:\n"
        "  -v, --debug - Print more information\n"
        "  -h  --help - print this\n"
        "  -T, --immediate-exit, Immediate exit for library linking test\n"
        "\n");
}

bool checkRunArgs(int argc, char *argv[], uint16_t &port,
                  string &appListFileName, bool &terminalMode)
{
    uint16_t portVal = 0;
    size_t portArgLen = 0;
    int opt = 0;
    bool wrongArg = false;
    char *portArgEnd = NULL;
    int long_index = 0;

    const struct option long_options[] = {
        {"help", no_argument, NULL, 'h'},
        {"terminal", no_argument, NULL, 't'},
        {"daemon", no_argument, NULL, 'd'},
        {"debug", no_argument, NULL, 'v'},
        {"config", required_argument, NULL, 'c'},
        {"noext", no_argument, NULL, 'n'},
        {"port", required_argument, NULL, 'p'},
        {"immediate-exit", no_argument, NULL, 'T'},
        {NULL, 0, NULL, 0}};

    while ((opt = getopt_long(argc, argv, "thdvTc:p:", long_options,
                              &long_index)) != -1)
    {
        switch (opt)
        {
        case 'h':
            print_help();
            exit(0);

        case 't':
            terminalMode = true;
            break;

        case 'd':
            terminalMode = false;
            break;

        case 'v':
            VERBOSE = true;
            break;

        case 'n':
            WITHOUT_RFMEASK = true;
            break;

        case 'T':
            nos::println("immediate exit");
            exit(0);

        case 'c':
        {
            size_t len = strlen(optarg);
            if (len > 5)
            {
                if (!strcmp(optarg + len - 5, ".json"))
                    appListFileName = string(optarg);
                else
                    wrongArg = true;
            }
            else
                wrongArg = true;
        }
        break;

        case 'p':
        {
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
        }
        break;

        default:
            print_help();
            exit(0);
        }
    }
    return 0;
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