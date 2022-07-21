#include "main.h"
#include "AppManager.h"
#include "RFDaemonServer.h"
#include <Beam.h>
#include <console.h>
#include <getopt.h>
#include <iostream>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>

bool VERBOSE = false;
bool WITHOUT_RFMEASK = false;
bool terminalMode = true;
int tcp_console_port = 5000;
uint16_t port = DEFAULT_TCP_PORT;
std::unique_ptr<AppManager> appManager = {};
std::unique_ptr<RFDaemonServer> srv = {};
std::unique_ptr<Beam> beam = {};
std::thread srvRxThread;
std::thread srvTxThread;

void interrupt_signal_handler(int signum);

void interrupt_child(int signum)
{
    (void)signum;
    int status;

    for (;;)
    {
        pid_t p = waitpid(-1, &status, WUNTRACED | WNOHANG);
        if (p < 0)
        {
            perror("waitpid");
            exit(1);
        }
        if (p == 0)
            break;

        appManager->on_child_finished(p);
    }
}

int main(int argc, char *argv[])
{
    std::string configFileName;
    pid_t daemonPid = 0;

    signal(SIGINT, interrupt_signal_handler);
    signal(SIGTERM, interrupt_signal_handler);
    signal(SIGSEGV, interrupt_signal_handler);
    signal(SIGCHLD, interrupt_child);

    checkRunArgs(argc, argv, port, configFileName, terminalMode);

    nos::fprintln("Starting in {} mode.\n",
                  terminalMode ? "terminal" : "server");

    beam = std::make_unique<Beam>();
    beam->start();

    if (!terminalMode)
        daemonPid = fork();
    if (daemonPid == -1) // main process code part
        nos::println("Error: RFDaemon process fork failed.");
    else if (daemonPid == 0) // fork process code part
    {
        srv = std::make_unique<RFDaemonServer>(port);
        appManager = std::make_unique<AppManager>(configFileName);

        int appfile_error = appManager->loadConfigFile();
        if (appfile_error)
        {
            nos::println(
                "Application script has errors. Server-only mode runned.");
        }
        appManager->runApps();

        start_tcp_console(tcp_console_port);
        srvRxThread = std::thread(tcpServerReceiveThreadHandler);
        srvTxThread = std::thread(tcpServerSendThreadHandler);
        if (terminalMode)
        {
            start_stdstream_console();
        }
        try
        {
            srvTxThread.join();
        }
        catch (const std::exception &e)
        {
            nos::println("Error: {}", e.what());
        }

        try
        {
            srvRxThread.join();
        }
        catch (const std::exception &e)
        {
            nos::println("Error: {}", e.what());
        }
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
                  std::string &appListFileName, bool &terminalMode)
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
                    appListFileName = std::string(optarg);
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
    srv->setAppManager(appManager.get());
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
    quick_exit(sig);
}

// TODO
void stop_world()
{
    appManager->closeApps();
}

void interrupt_signal_handler(int sig)
{
    nos::println("Interrupt signal received.");
    stop_world();
    exitHandler(sig);
}
