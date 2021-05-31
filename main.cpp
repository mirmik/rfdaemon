﻿#include "main.h"
#include <getopt.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <signal.h>
#include <thread>
#include "RFDaemonServer.h"
#include "AppManager.h"

using namespace std;

uint16_t port = DEFAULT_TCP_PORT;
AppManager* appManager = NULL;
RFDaemonServer* srv = NULL;
thread srvRxThread;
thread srvTxThread;
thread userThread;

/*
1. Проверяем параметры запуска и если в них ошибка - стартуем с дефолтными параметрами
2. Пробуем читать файл конфигурации демона (файл json со списком запускаемых программ)
3. Если файл отсутствует или в нем ошибки, то не запускаем программы (server-only mode)
4. Если файл в порядке то запускаем из него программы, каждую в отдельном процессе fork->exec
    и сохраняем в память PID всех запущенных процессов
5. Запускаем сетевое соединение в отдельном потоке, в котором ждем поступающих запросов и отвечаем на них
6. В родительском потоке постоянно проверяем состояние запущенных программ.
    Если какая то упала, то перезапускаем.
*/
int main(int argc, char* argv[])
{
    string configFileName;
    bool terminalMode = false, serverOnlyMode = false;
    pid_t daemonPid = 0;

    system("pkill rfmeas");
    system("pkill dataproxy");
    system("pkill ctrans");
    system("pkill crowker");
    //system("pkill ConsoleApp");

    if (checkRunArgs(argc, argv, port, configFileName, terminalMode))
    {
#warning "terminalMode = true only for debug"
        terminalMode = true;
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
        signal(SIGINT, exitHandler);
        signal(SIGTERM, exitHandler);
        signal(SIGQUIT, exitHandler);
        srv = new RFDaemonServer(port);
        appManager = new AppManager(configFileName);
        if (appManager->loadConfigFile())
        {
            serverOnlyMode = true;
            cout << "Run server-only mode.\n";
        }
        if (!serverOnlyMode)
            appManager->runApps();

        srvRxThread = thread(tcpServerReceiveThread);
        srvTxThread = thread(tcpServerSendThread);
        if (terminalMode)
        {
            userThread = thread(userIOThread);
            userThread.join();
        }
        srvTxThread.join();
        srvRxThread.join();
    }
    return 0;
}

bool checkRunArgs(int argc, char* argv[], uint16_t& port, string& appListFileName, bool& terminalMode)
{
    int opt = 0, len = 0, portArgLen = 0;
    bool wrongArg = false;
    char* portArgEnd = NULL;

    while ((opt = getopt(argc, argv, "tc:p:")) != -1)
    {
        switch (opt)
        {
        case 't':
            terminalMode = 1;
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
            port = strtoul(optarg, &portArgEnd, 10);

            if ((portArgEnd - optarg) < portArgLen)
            {
                port = strtoul(optarg, &portArgEnd, 16);
                wrongArg = (portArgEnd - optarg) < portArgLen;
            }
            break;
        default:
            wrongArg = true;
        }
    }
    if (wrongArg || !len)
        printf("Usage: %s [-p 0..65535/0..FFFF] [-t] -c filename.json\n", argv[0]);
    return (wrongArg || !len);
}

int tcpServerSendThread()
{
    srv->setAppManager(appManager);
    return srv->sendThread();
}

int tcpServerReceiveThread()
{
    while (!srv);
    return srv->receiveThread();
}

int userIOThread()
{
    string userInput;
    while (1)
    {
        getline(cin, userInput);
        if (userInput.find("get ", 0) == 0)
        {
            if (userInput.find("tasks", 4) == 4)
            {
                const auto& apps = appManager->getAppsList();
                bool verbose = userInput.find(" -v", 9) == 9;
                for (const auto& app : apps)
                {
                    cout << app.name() << "\t" << (app.stopped() ? "Stopped" : "Active");
                    if (verbose)
                        cout << "\t" << app.pid() << "\t" << app.command() << "\t" << app.uptime() << endl;
                    else
                        cout << endl;
                }
            }
        }
        else if (userInput.find("restart", 0) == 0)
            appManager->restartApps();
        else if (userInput.find("stop", 0) == 0)
            appManager->closeApps();
        else if (userInput.find("connstatus", 0) == 0)
            cout << (srv->clientConnected() ? ("Connected to " + srv->getClientInfo()) : "Disconnected") << endl;
        else
            cout << "Wrong command." << endl;
    }
    return 0;
}

void exitHandler(int sig)
{
    delete srv;
    appManager->closeApps();
    delete appManager;
    exit(sig);
}
