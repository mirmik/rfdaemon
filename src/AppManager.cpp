#include <cctype>
#include <iostream>
#include <fstream>
#include <mutex>
#include <unistd.h>
#include "jsoncpp/json/json.h"
#include "AppManager.h"
#include "zlib.h"

std::mutex AppManager::ioMutex;
using namespace std::chrono_literals;

AppManager::AppManager(const std::string& appListFileName)
{
    appFilename = appListFileName;
    spamserver.start(5001);
}

void AppManager::send_spam(const std::string& message)
{
    std::lock_guard<std::mutex> lock(spam_mutex);
    spamserver.print(message);
}

void AppManager::send_spam(const std::vector<uint8_t>& message)
{
    std::lock_guard<std::mutex> lock(spam_mutex);
    spamserver.write(message.data(), message.size());
}

bool AppManager::loadConfigFile()
{
    Json::Value root;
    Json::CharReaderBuilder builder;
    JSONCPP_STRING errs;
    std::ifstream appFile(appFilename);
    bool error = false;
    if (appFile.is_open())
        nos::println("RFDaemon configuration file '{}' found.", appFilename);
    else
    {
        nos::println("RFDaemon configuration file '{}' missing.", appFilename);
        pushError(Errors::AppListNotFound);
        return true;
    }

    if (!parseFromStream(builder, appFile, &root, &errs))
    {
        nos::println("Errors in configuration file '{}' ['{}'].", appFilename, errs);
        pushError(Errors::AppListSyntaxError);
        error = true;
    }
    else
    {
        int arraySize = root["apps"].size();
        if (arraySize)
        {
            closeApps();
            apps.clear();

            std::map<int, int> orderList;
            for (int i = 0; i < arraySize; i++)
                orderList[i] = root["apps"][i]["order"].asInt() - 1;

            for (int i = 0; i < arraySize; i++)
            {
                std::vector<LinkedFile> linked_files;
                int order = orderList[orderList[i]];
                std::string name = root["apps"][order]["name"].asString();
                std::string cmd = root["apps"][order]["command"].asString();
                auto logs = root["apps"][order]["logs"];
                auto files = root["apps"][order]["files"];

                if (!cmd.empty() && !name.empty())
                {
                    App::RestartMode restartMode;
                    if (root["apps"][order]["restart"].asString() == "error")
                        restartMode = App::RestartMode::ERROR;
                    else if (root["apps"][order]["restart"].asString() == "never")
                        restartMode = App::RestartMode::NEVER;
                    else
                        restartMode = App::RestartMode::ALWAYS;

                    if (!files.empty())
                    {
                        for (const auto& rec : root["apps"][order]["files"]) 
                        {
                            LinkedFile file;
                            file.path = rec["path"].asString();
                            file.name = rec["name"].asString();
                            file.editable = rec["editable"].asBool();
                            linked_files.push_back(file);
                        }
                    }
                    apps.emplace_back(apps.size(), name, cmd, restartMode, linked_files);
                }
                else
                {
                    error = true;
                    break;
                }
            }
        }
        else
        {
            nos::fprintln("No apps found in file '{}'.", appFilename);
            error = true;
        }
        
        int sysLogCount = root["sys_logs"].size();
        for (int i = 0; i < sysLogCount; i++)
            systemLogPaths.push_back(root["sys_logs"][i].asString());
    }

    return error;
}

void AppManager::runApps()
{
    for (auto& a : apps)
    {
        if (a.stopped())
            a.start();
    }
}

void AppManager::closeApps()
{
    for (auto& a : apps)
    {
        if (!a.stopped())
            a.stop();
    }
    std::lock_guard lock(ioMutex);
    nos::println("All created processes have just been killed.");
}

void AppManager::restartApps()
{
    closeApps();
    std::this_thread::sleep_for(500ms);
    runApps();
}

size_t AppManager::getAppCount() const
{
    return apps.size();
}

const std::string& AppManager::getAppConfigFilename()
{
    return appFilename;
}

std::vector<App>& AppManager::getAppsList()
{
    return apps;
}

App* AppManager::findApp(const std::string& name)
{
    if (std::isdigit(name[0])) 
    {
        return &apps[stoi(name)];
    }

    for (auto& a : apps)
    {
        if (!a.name().compare(name))
            return &a;
    }
    return nullptr;
}

App* AppManager::getApp(size_t index)
{
    if (index < apps.size())
        return &apps[index];
    
    return nullptr;
}

const std::string& AppManager::getDeviceDescFilename() const
{
    return settingsFilename;
}

const std::string& AppManager::getDeviceRuntimeFilename() const
{
    return runtimeSettingsFilename;
}

std::list<uint8_t>& AppManager::errors()
{
    return errorList;
}

std::vector<std::string>& AppManager::getSystemLogPaths()
{
    return systemLogPaths;
}

void AppManager::pushError(Errors error)
{
    errorList.push_back((uint8_t)error);
    if (errorList.size() > 100)
        errorList.pop_front();
}

std::vector<AppManager::Log> AppManager::packLogs()
{
    std::vector<AppManager::Log> data;
    std::vector<std::string> paths = systemLogPaths;
    for (const auto& app : apps)
    {
        auto logPaths = app.logPaths();
        if (!logPaths.empty())
        {
            for (const auto& path : logPaths)
                paths.push_back(path);
        }
    }

    for (const auto& path : paths)
    {
        std::ifstream f(path);
        if (f.is_open())
        {
            std::string s(std::istreambuf_iterator<char>{f}, {});
            size_t fileSize = s.size();
            size_t packedSize = fileSize;
            std::vector<uint8_t> output(fileSize);
            if (compress(output.data(), &packedSize, (Bytef*)s.data(), fileSize) == Z_OK)
            {
                std::vector<uint8_t> packed(4);
                *(uint32_t*)(packed.data()) = __bswap_32((uint32_t)packedSize);
                packed.insert(packed.end(), output.begin(), output.begin() + packedSize);
                data.push_back({ path, packed });
            }
        }
    }
    return data;
}
