#include <cstdio>
#include <fstream>
#include <iostream>
#include <atomic>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <boost/algorithm/string.hpp>
#include "RFDaemon.h"
#include "main.h"
#include "pthread.h"
#include "jsoncpp/json/json.h"

using namespace std;

vector<int> appRestartEnableList;
vector<int> appRestartAttempts;
vector<pid_t> appPidList;
vector<string> appList, appCmdList;
vector<vector<string>> appCmdArgList;
int srvRetCode = 0, appWatcherRetCode = 0, userIORetCode = 0;

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
    uint16_t port = DEFAULT_TCP_PORT;
    uint8_t terminalMode = 0;
    bool serverOnlyMode = false;
    pid_t daemonPid = 0;
    atomic<uint32_t> srvThreadArg = 0;
    atomic<uint32_t> appWatcherThreadArg = 0;
    atomic<uint32_t> userIOThreadArg = 0;
    pthread_t hNetworkThread = 0, hAppWatcherThread = 0, hUserIOThread = 0;

    if (checkRunArgs(argc, argv, port, configFileName, terminalMode))
    {
        terminalMode = false;
        configFileName = "applist.json";
        port = DEFAULT_TCP_PORT;
    }

    if (checkConfigFile(configFileName, appList, appCmdList, appCmdArgList))
    {
        serverOnlyMode = true;
        cout << "Run server-only mode.\n";
    }
    
    cout << "Starting in " << (terminalMode ? "terminal" : "daemon") << "mode.\n";

    if (!terminalMode)
        daemonPid = fork();
    if (daemonPid == -1) // main process code part
        cout << "Error: RFDaemon process fork failed.";
    else if (!daemonPid) // fork process code part
    {
        if (!serverOnlyMode)
            appPidList = runApps(appCmdList, appCmdArgList);
        //sleep(1);
        //closeApps(appPidList);

        pthread_create(&hNetworkThread, NULL, tcpServerThread, &srvThreadArg);
        pthread_create(&hAppWatcherThread, NULL, appWatcherThread, &appWatcherThreadArg);
        if (terminalMode)
            pthread_create(&hUserIOThread, NULL, userIOThread, &userIOThreadArg);
        pthread_join(hNetworkThread, NULL);
        pthread_join(hAppWatcherThread, NULL);
        if (terminalMode)
            pthread_join(hUserIOThread, NULL);
        if (srvRetCode || appWatcherRetCode || userIORetCode)
            cerr << srvRetCode << " " << appWatcherRetCode << " " << userIORetCode << endl;
    }
    return 0;
}

bool checkRunArgs(int argc, char* argv[], uint16_t& port, string& appList, uint8_t& terminalMode)
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
                    appList = string(optarg);
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
        printf("Usage: %s [-p 0..65535/0..FFFF] [-t] -c file.json\n", argv[0]);
    return (wrongArg || !len);
}

bool checkConfigFile(const string& filename, vector<string>& apps, vector<string>& cmds, vector<vector<string>>& cmdArgs)
{
    Json::Value root;
    Json::CharReaderBuilder builder;
    JSONCPP_STRING errs;
    bool success = false;
    ifstream appFile(filename);

    if (!appFile.is_open())
    {
        cout << "RFDaemon configuration file \"" + filename + "\" missing.\n";
        success = true;
    }
    else
        cout << "RFDaemon configuration file \"" + filename + "\" found.\n";

    if (!parseFromStream(builder, appFile, &root, &errs))
    {
        cout << "Errors in configuration file \"" + filename + "\"[" << errs << "].\n";
        success = true;
    }
    else
    {
        int arraySize = root["apps"].size();
        if (!arraySize)
        {
            cout << "No apps found in file \"" + filename + "\".\n";
            success = true;
        }
        for (int i = 0; i < arraySize; i++)
        {
            apps.push_back(root["apps"][i]["name"].asString());
            cmds.push_back(root["apps"][i]["command"].asString());
        }
        if (apps.empty())
        {
            cout << "No apps found in file \"" + filename + "\"\n";
            success = true;
        }
        if (apps.size() != cmds.size())
        {
            cout << "Property 'command' missing in file \"" + filename + "\".\n";
            success = true;
        }

        for (int i = 0; i < arraySize; i++)
        {
            string& s = cmds[i];

            if (!s.empty())
            {
                size_t argsBegin = s.find_first_of(' ');
                if (argsBegin != string::npos)
                {
                    vector<string> args;
                    string argstr = s.substr(argsBegin);
                    boost::split(args, argstr, boost::is_any_of(" "), boost::algorithm::token_compress_mode_type::token_compress_on);
                    cmdArgs.push_back(args);
                    s = s.substr(0, argsBegin);
                }
                else
                    cmdArgs.push_back(vector<string>({ "" }));
            }
            cmdArgs[i][0] = s;
            appRestartEnableList.push_back(1);
        }
        appRestartAttempts.resize(arraySize);
    }
    return success;
}

/*
* Executes each string in cmds as executable with arguments in cmdArgs
* Size of cmds must be equal to size of cmdArgs
* Returns vector with pid of created executable's processes
*/
vector<pid_t> runApps(const vector<string>& cmds, const vector<vector<string>>& cmdArgs)
{
    size_t cmdCount = cmds.size();
    vector<vector<const char*>> args(cmdCount);
    vector<pid_t> pids;

    for (size_t i = 0; i < cmdCount; i++)
    {
        for (auto& item : cmdArgs[i])
            args[i].push_back(item.c_str());
        args[i].push_back(NULL);
    }

    for (size_t i = 0; i < cmdCount; i++)
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            int status = execvp(cmds[i].c_str(), (char**)args[i].data());
            cout << "App \"" << cmds[i] << "\" exit status: " << status << endl;
            exit(status);
        }
        else if (pid > 0)
            pids.push_back(pid);
        else
            cout << "Error: failed to start process " << cmds[i] << "." << endl;
        cout << pid << " " << "parent: " << getpid() << endl;
    }
    return pids;
}

pid_t runApp(const string& cmd, const vector<string>& args)
{
    vector<const char*> argsStr;
    pid_t pid = 0;

    for (auto& item : args)
        argsStr.push_back(item.c_str());
    argsStr.push_back(NULL);

    pid = fork();
    if (pid == 0)
    {
        int status = execvp(cmd.c_str(), (char**)argsStr.data());
        cout << "App \"" << cmd << "\" exit status: " << status << endl;
        exit(status);
    }
    else if (pid < 0)
        cout << "Error: failed to start process " << cmd << "." << endl;
    cout << pid << " " << "parent: " << getpid() << endl;
    return pid;
}

void closeApps(const vector<pid_t>& appPidList)
{
    for (auto& p : appPidList)
        kill(p, SIGTERM);
    cout << "All created processes have just been killed." << endl;
}

void* tcpServerThread(void* arg)
{
    //RFDaemon* tcpServer = new RFDaemon();
    //tcpServer->exec();
    return &srvRetCode;
}

void* appWatcherThread(void* arg)
{
    while (1)
    {
        int sumAttempts = 0;
        for (auto& a : appRestartAttempts)
            sumAttempts += a;

        usleep(APP_CHECK_INTERVAL_US + sumAttempts * APP_CHECK_INTERVAL_US);

        for (size_t i = 0; i < appPidList.size(); i++)
        {
            if (appRestartEnableList[i] != 0)
            {
                if (kill(appPidList[i], 0) == -1)
                {
                    appPidList[i] = runApp(appCmdList[i], appCmdArgList[i]);
                    if (appPidList[i] == -1)
                    {
                        if (appRestartAttempts[i] < APP_MAX_SUCCESSIVE_RESTART_ATTEMPTS)
                            appRestartAttempts[i]++;
                        else
                        {
                            appRestartEnableList[i] = 0;
                            appRestartAttempts[i] = 0;
                        }
                    }
                    else
                        appRestartAttempts[i] = 0;
                }
            }
        }
    }
    return &appWatcherRetCode;
}

void* userIOThread(void* arg)
{
    while (1)
    {

    }
    return &userIORetCode;
}
