#include <string.h>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <atomic>
#include <unistd.h>
#include <signal.h>
#include "RFDaemon.h"
#include "main.h"
#include "pthread.h"
#include "RFDaemonServer.h"
#include "AppManager.h"
#include "DeviceManager.h"

using namespace std;

uint16_t port = DEFAULT_TCP_PORT;
AppManager appManager;
DeviceManager* devManager = NULL;
RFDaemonServer* srv = NULL;
int srvSendRetCode = 0, srvRecvRetCode = 0,
clientSendRetCode = 0, clientRecvRetCode = 0,
appWatcherRetCode = 0, userIORetCode = 0;

void exitHandler(int sig);

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
    string configFileName = "";
    uint8_t terminalMode = 0;
    bool serverOnlyMode = false;
    pid_t daemonPid = 0;
    uint32_t srvSendThreadArg = 0, srvRecvThreadArg = 0,
        clientSendThreadArg = 0, clientRecvThreadArg = 0,
        appWatcherThreadArg = 0, userIOThreadArg = 0;
    pthread_t hSrvSendThread = 0, hSrvRecvThread = 0,
        hClientSendThread = 0, hClientRecvThread = 0,
        hAppWatcherThread = 0, hUserIOThread = 0;

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

    if (appManager.openConfigFile(configFileName))
    {
        serverOnlyMode = true;
        cout << "Run server-only mode.\n";
    }

    devManager = new DeviceManager(appManager.getDeviceDescFilename());
    srv = new RFDaemonServer(port);
    srv->setAppManager(&appManager);
    srv->setDeviceManager(devManager);

    cout << "Starting in " << (terminalMode ? "terminal" : "daemon") << "mode.\n";

    signal(SIGINT, exitHandler);

    if (!terminalMode)
        daemonPid = fork();
    if (daemonPid == -1) // main process code part
        cout << "Error: RFDaemon process fork failed.";
    else if (!daemonPid) // fork process code part
    {
        if (!serverOnlyMode)
            appManager.runApps();
        //sleep(1);
        //closeApps(appPidList);

        pthread_create(&hSrvRecvThread, NULL, tcpServerReceiveThread, &srvRecvThreadArg);
        pthread_create(&hSrvSendThread, NULL, tcpServerSendThread, &srvSendThreadArg);
        pthread_create(&hClientRecvThread, NULL, tcpClientReceiveThread, &clientRecvThreadArg);
        pthread_create(&hClientSendThread, NULL, tcpClientSendThread, &clientSendThreadArg);
        pthread_create(&hAppWatcherThread, NULL, appWatcherThread, &appWatcherThreadArg);
        if (terminalMode)
        {
            pthread_create(&hUserIOThread, NULL, userIOThread, &userIOThreadArg);
            pthread_join(hUserIOThread, NULL);
        }
        pthread_join(hSrvRecvThread, NULL);
        pthread_join(hSrvSendThread, NULL);
        pthread_join(hClientRecvThread, NULL);
        pthread_join(hClientSendThread, NULL);
        pthread_join(hAppWatcherThread, NULL);
        if (srvSendRetCode || srvRecvRetCode || clientSendRetCode || clientRecvRetCode || appWatcherRetCode || userIORetCode)
            cerr << srvSendRetCode << " "
            << srvRecvRetCode << " "
            << clientSendRetCode << " "
            << clientRecvRetCode << " "
            << appWatcherRetCode << " "
            << userIORetCode << endl;
    }
    return 0;
}

bool checkRunArgs(int argc, char* argv[], uint16_t& port, string& appListFileName, uint8_t& terminalMode)
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

void* tcpServerSendThread(void* arg)
{
    srvSendRetCode = srv->sendThread();
    return &srvSendRetCode;
}

void* tcpServerReceiveThread(void* arg)
{
    srvRecvRetCode = srv->receiveThread();
    return &srvRecvRetCode;
}

void* tcpClientSendThread(void* arg)
{
    clientSendRetCode = devManager->sendThread();
    return &clientSendRetCode;
}

void* tcpClientReceiveThread(void* arg)
{
    usleep(1000000);
    devManager->connectToPort("localhost", RFMEASK_TCP_PORT);
    clientRecvRetCode = devManager->receiveThread();
    return &clientRecvRetCode;
}

void* appWatcherThread(void* arg)
{
    appWatcherRetCode = appManager.thread();
    return &appWatcherRetCode;
}

void* userIOThread(void* arg)
{
    srand(2341234);
    while (1)
    {
        sleep(3);
        devManager->setAxisPosition(0, 5);
        for (int i = 0; i < 50; i++)
        {
            usleep(100000);
            printf("%.3f\n", devManager->getAxisPos(0));
        }
        while (true)
        {
            devManager->setMeasuredValue(0, (rand() % 1000) / 1000.0 + 3);
            devManager->setMeasuredValue(1, (rand() % 1000) / 1000.0 + 6);
            devManager->setMeasuredValue(2, (rand() % 1000) / 1000.0 + 8);
            devManager->setMeasuredValue(3, (rand() % 1000) / 1000.0 + 11);
            usleep(1000);
        }
        //printf("Shutting down in 3 seconds...\n");
        //sleep(3);
        //devManager->disconnect();
        //appManager.closeApps();
        break;
    }
    return &userIORetCode;
}

void exitHandler(int sig)
{
    appManager.closeApps();
    exit(sig);
}
