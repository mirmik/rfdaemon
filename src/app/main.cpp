#include "main.h"
#include "AppManager.h"
#include "RFDaemonServer.h"
#include <console.h>
#include <defs.h>
#include <getopt.h>
#include <httpserver.h>
#include <iostream>
#include <signal.h>
#include <shutdown.h>
#include <string.h>
#include <version.h>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>

bool VERBOSE = false;
bool TERMINAL_MODE = true;
bool NOCONSOLE_MODE = false;
bool EDIT_MODE = false;
bool USE_LEGACY_API_PORT = true;
int API_CONSOLE_PORT = RFDAEMON_DEFAULT_API_PORT;
uint16_t RFDAEMON_PORT = DEFAULT_RFDAEMON_PROTO_PORT;
bool ENABLE_HTTP_SERVER = false;
std::string HTTP_SERVER_HOST = "127.0.0.1";
int HTTP_SERVER_PORT = 8080;
bool PRINT_LOGS = false;

std::unique_ptr<AppManager> appManager = {};
std::unique_ptr<RFDaemonServer> srv = {};
std::thread srvRxThread;
std::thread systemd_updater_thread;
std::string APPLICATION_LIST_FILE_NAME = "/etc/rfdaemon/applications.json";

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
            nos::println(status, p);
            perror("waitpid");
            exit(1);
        }
        if (p == 0)
            break;

        appManager->on_child_finished(p);
    }
}

void proc_exit(int sig)
{
    (void)sig;
    int retcode;

    while (true)
    {
        pid_t pid = wait3(&retcode, WNOHANG, (struct rusage *)NULL);
        if (pid == 0 || pid == -1)
        {
            // it is not a error
            return;
        }
        nos::fprintln("Proccess {} was stopped. (SIGCHLD)", pid);
        appManager->on_child_finished(pid);
    }
}

int main(int argc, char *argv[])
{
    pid_t daemonPid = 0;

    signal(SIGINT, interrupt_signal_handler);
    signal(SIGTERM, interrupt_signal_handler);
    signal(SIGSEGV, interrupt_signal_handler);
    signal(SIGCHLD, proc_exit);

    checkRunArgs(argc, argv);

    if (EDIT_MODE)
    {
        nos::println("Edit mode");
        std::vector<char *> args = {(char *)"/usr/bin/nano",
                                    (char *)APPLICATION_LIST_FILE_NAME.c_str(),
                                    0};
        int sts = execve(args[0], args.data(), {});
        nos::println("execve status:", sts);
        perror("execve");
        exit(0);
    }

    nos::fprintln("Starting in {} mode.\n",
                  TERMINAL_MODE ? "terminal" : "server");

    if (!TERMINAL_MODE)
        daemonPid = fork();
    if (daemonPid == -1) // main process code part
        nos::println("Error: RFDaemon process fork failed.");
    else if (daemonPid == 0) // fork process code part
    {
        try
        {
            nos::fprintln("Starting RFDaemon legacy server on port {}",
                          RFDAEMON_PORT);
            srv = std::make_unique<RFDaemonServer>(RFDAEMON_PORT);
        }
        catch (const std::exception &ex)
        {
            nos::fprintln(
                "Server exception (maybe the daemon is already running): {}",
                ex.what());
        }

        appManager = std::make_unique<AppManager>(APPLICATION_LIST_FILE_NAME);

        int appfile_error = appManager->loadConfigFile();
        if (appfile_error)
        {
            nos::println(
                "Application script has errors. Server-only mode runned.");
        }
        appManager->runApps();

        if (ENABLE_HTTP_SERVER)
        {
            nos::fprintln("Starting HTTP server on {}:{}", HTTP_SERVER_HOST, HTTP_SERVER_PORT);
            start_httpserver(HTTP_SERVER_HOST, HTTP_SERVER_PORT);
        }

        start_tcp_console(API_CONSOLE_PORT);
        if (USE_LEGACY_API_PORT)
        {
            nos::fprintln("Legacy API port {} is used", 5000);
            start_tcp_console(5000);
        }

        nos::println("RFDaemon started");
        srvRxThread = std::thread(tcpServerReceiveThreadHandler, srv.get(),
                                  appManager.get());

        nos::println("Starting systemd updater thread");
        systemd_updater_thread = std::thread(
            &AppManager::update_systemctl_projects_status,
            appManager.get());

        if (TERMINAL_MODE && !NOCONSOLE_MODE)
        {
            start_stdstream_console();
        }

        // Wait for shutdown signal
        nos::println("Waiting for shutdown signal...");
        while (!is_shutdown_requested())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        nos::println("Shutdown requested, cleaning up...");

        // Stop all servers first
        nos::println("[shutdown] Stopping HTTP server...");
        stop_httpserver();
        nos::println("[shutdown] HTTP server stopped");

        nos::println("[shutdown] Stopping TCP consoles...");
        stop_tcp_consoles();
        nos::println("[shutdown] TCP consoles stopped");

        // Stop managed applications
        nos::println("[shutdown] Closing apps...");
        if (appManager)
        {
            appManager->closeApps();
        }
        nos::println("[shutdown] Apps closed");

        // Join threads
        nos::println("[shutdown] Joining systemd_updater_thread...");
        if (systemd_updater_thread.joinable())
        {
            systemd_updater_thread.join();
        }
        nos::println("[shutdown] systemd_updater_thread joined");

        nos::println("[shutdown] Stopping RFDaemon server...");
        if (srv)
        {
            srv->stop();
        }
        nos::println("[shutdown] RFDaemon server stopped");

        nos::println("[shutdown] Joining srvRxThread...");
        if (srvRxThread.joinable())
        {
            srvRxThread.join();
        }
        nos::println("[shutdown] srvRxThread joined");

        nos::println("[shutdown] Joining TCP console threads...");
        join_tcp_console_threads();
        nos::println("[shutdown] TCP console threads joined");

        nos::println("Shutdown complete.");
    }
    return 0;
}

void print_help()
{
    nos::fprint(
        "Usage: rfdaemon [OPTIONS]...\n"
        "\n"
        "RFDaemon - lightweight process manager for running and monitoring applications.\n"
        "\n"
        "OPTIONS:\n"
        "  General:\n"
        "    -h, --help                  Show this help message and exit\n"
        "    -V, --version               Print version and exit\n"
        "    -v, --verbose               Enable verbose output\n"
        "\n"
        "  Configuration:\n"
        "    -c, --config <FILE>         Path to applications config file\n"
        "                                (default: /etc/rfdaemon/applications.json)\n"
        "    -e, --edit                  Open config file in editor (nano)\n"
        "\n"
        "  Execution mode:\n"
        "    -t, --terminal              Run in terminal/foreground mode (default)\n"
        "    -d, --daemon                Run as background daemon\n"
        "    -N, --noconsole             Disable interactive console in terminal mode\n"
        "\n"
        "  Network:\n"
        "    -p, --port <PORT>           RFDaemon protocol port (default: 5125)\n"
        "        --http <HOST:PORT>      Enable HTTP server on specified address\n"
        "                                Example: --http 0.0.0.0:8080\n"
        "                                (HTTP server is disabled by default)\n"
        "    -A, --api-port <PORT>       API console port (default: 15129)\n"
        "    -u, --nolegacyapi           Disable legacy API on port 5000\n"
        "    -U, --uselegacyapi          Enable legacy API on port 5000 (default)\n"
        "\n"
        "  Debug:\n"
        "    -P, --print-logs            Print application logs to stdout\n"
        "    -T, --immediate-exit        Exit immediately (for testing)\n"
        "\n"
        "EXAMPLES:\n"
        "  rfdaemon -c /path/to/apps.json\n"
        "  rfdaemon --http 127.0.0.1:8080\n"
        "  rfdaemon --http 0.0.0.0:9000 -d\n"
        "\n");
}

bool checkRunArgs(int argc, char *argv[])
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
        {"verbose", no_argument, NULL, 'v'},
        {"config", required_argument, NULL, 'c'},
        {"noext", no_argument, NULL, 'n'},
        {"noconsole", no_argument, NULL, 'N'},
        {"port", required_argument, NULL, 'p'},
        {"edit", no_argument, NULL, 'e'},
        {"immediate-exit", no_argument, NULL, 'T'},
        {"version", no_argument, NULL, 'V'},
        {"http", required_argument, NULL, 'H'},
        {"nolegacyapi", no_argument, NULL, 'u'},
        {"uselegacyapi", no_argument, NULL, 'U'},
        {"api-port", required_argument, NULL, 'A'},
        {"print-logs", no_argument, NULL, 'P'},
        {NULL, 0, NULL, 0}};

    while ((opt = getopt_long(argc, argv, "htdvc:np:eTV::NuUH:A:P", long_options,
                              &long_index)) != -1)
    {
        switch (opt)
        {
        case 'h':
            print_help();
            exit(0);

        case 't':
            TERMINAL_MODE = true;
            break;

        case 'd':
            TERMINAL_MODE = false;
            break;

        case 'v':
            VERBOSE = true;
            break;

        case 'e':
            EDIT_MODE = true;
            break;

        case 'N':
            NOCONSOLE_MODE = true;
            break;

        case 'P':
            PRINT_LOGS = true;
            break;

        case 'T':
            nos::println("immediate exit");
            exit(0);

        case 'V':
            nos::fprintln("RFDaemon version: {}", VERSION);
            exit(0);

        case 'c':
            APPLICATION_LIST_FILE_NAME = std::string(optarg);
            break;

        case 'H':
        {
            ENABLE_HTTP_SERVER = true;
            std::string http_arg(optarg);
            size_t colon_pos = http_arg.rfind(':');
            if (colon_pos != std::string::npos)
            {
                HTTP_SERVER_HOST = http_arg.substr(0, colon_pos);
                HTTP_SERVER_PORT = std::stoi(http_arg.substr(colon_pos + 1));
            }
            else
            {
                // Only port specified
                HTTP_SERVER_PORT = std::stoi(http_arg);
            }
        }
        break;

        case 'A':
            API_CONSOLE_PORT = std::stoi(std::string(optarg));
            break;

        case 'U':
            USE_LEGACY_API_PORT = true;
            break;

        case 'u':
            USE_LEGACY_API_PORT = false;
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
                    RFDAEMON_PORT = portVal;
            }
            else
                RFDAEMON_PORT = portVal;
        }
        break;

        default:
            print_help();
            exit(0);
        }
    }
    return 0;
}

int tcpServerReceiveThreadHandler(RFDaemonServer *srv, AppManager *)
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
    stop_httpserver();
    appManager->closeApps();
}

void interrupt_signal_handler(int sig)
{
    (void)sig;
    // Only set the flag - this is async-signal-safe
    // The main thread will handle cleanup
    g_shutdown_requested.store(true, std::memory_order_release);
}
