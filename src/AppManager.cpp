#include "AppManager.h"
#include <byteswap.h>
#include <cctype>
#include <fstream>
#include <iostream>
#include <mutex>
#include <nos/trent/json.h>
#include <nos/trent/json_print.h>
#include <unistd.h>
#include <memory>

extern bool VERBOSE;
std::mutex AppManager::ioMutex;
AppManager * AppManager::_instance;
using namespace std::chrono_literals;

AppManager::AppManager(const std::string &appListFileName)
{
    nos::println("AppManager created");
    appFilename = appListFileName;
    spamserver.start(5001);
    _instance = this;
}

void AppManager::send_spam(const std::string &message)
{
    std::lock_guard<std::mutex> lock(spam_mutex);
    nos::print_to(spamserver, message);
}

void AppManager::send_spam(const std::vector<uint8_t> &message)
{
    std::lock_guard<std::mutex> lock(spam_mutex);
    spamserver.write(message.data(), message.size());
}

bool AppManager::loadConfigFile()
{
    if (VERBOSE)
        nos::println("loading config file from", appFilename);

    nos::trent root;
    try
    {
        // read json file
        std::string text;
        std::ifstream file(appFilename, std::ios::in);
        if (!file.is_open())
        {
            nos::println("failed to open config file", appFilename);
            perror("read");
            return false;
        }
        // read all text from file
        std::string line;
        while (std::getline(file, line))
        {
            text += line;
        }
        if (VERBOSE)
        {
            nos::println("config file content:\n", text);
        }

        root = nos::json::parse_file(appFilename);
    }
    catch (const std::exception &ex)
    {
        nos::println("Error while parsing app list file: ", ex.what());
        pushError(AppListConfigPath);
        return true;
    }

    bool error = false;
    int arraySize = root["apps"].as_list().size();
    if (arraySize)
    {
        closeApps();
        apps.clear();

        nos::println("Parse application list:");
        for (int i = 0; i < arraySize; i++)
        {
            auto &apptrent = root["apps"][i];

            std::vector<LinkedFile> linked_files;
            std::string name = apptrent["name"].as_string();
            if (name.empty())
            {
                nos::println("Error: app name is empty");
                continue;
            }

            std::string cmd = apptrent["command"].as_string();
            if (name.empty())
            {
                nos::println("Error: app command is empty");
                continue;
            }

            std::string user = apptrent["user"].as_string();
            auto logs = apptrent["logs"];
            auto files = apptrent["files"].as_list();
            auto env = apptrent["env"].as_dict();
            nos::fprintln("\t{}", name);

            App::RestartMode restartMode;
            if (apptrent["restart"].as_string() == "once")
            {
                restartMode = App::RestartMode::ONCE;
            }
            else if (apptrent["restart"].as_string() == "always")
            {
                restartMode = App::RestartMode::ALWAYS;
            }
            else
            {
                nos::println("Unknown restart mode for app", name);
                restartMode = App::RestartMode::ONCE;
            }

            if (!files.empty())
            {
                for (const auto &rec : apptrent["files"].as_list())
                {
                    LinkedFile file;
                    file.path = rec["path"].as_string();
                    file.name = rec["name"].as_string();
                    file.editable = rec["editable"].as_bool();
                    linked_files.push_back(file);
                }
            }

            std::string systemd_bind;
            if (apptrent["systemd_bind"].is_string()) 
            {
                systemd_bind = apptrent["systemd_bind"].as_string();
            }

            apps.emplace_back(
                std::make_shared<App>(apps.size(), name, cmd, restartMode, linked_files,
                              user));
            auto &app = apps.back();
            app->set_systemd_bind(systemd_bind);

            std::unordered_map<std::string, std::string> env_map;
            for (const auto &rec : env)
                env_map[rec.first] = rec.second.as_string();
            app->set_environment_variables(env_map);
        }
    }
    else
    {
        nos::fprintln("No apps found in file '{}'.", appFilename);
        error = true;
    }

    int sysLogCount = root["sys_logs"].as_list().size();
    for (int i = 0; i < sysLogCount; i++)
        systemLogPaths.push_back(root["sys_logs"][i].as_string());

    return error;
}

void AppManager::runApps()
{
    std::lock_guard<std::mutex> lock(apps_mutex);
    for (auto &a : apps)
    {
        if (a->stopped())
            a->start();
    }
}

void AppManager::closeApps()
{
    std::lock_guard<std::mutex> lock(apps_mutex);
    for (auto &a : apps)
    {
        if (!a->stopped())
            a->stop();
    }

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
    std::lock_guard<std::mutex> lock(apps_mutex);
    return apps.size();
}

const std::string &AppManager::getAppConfigFilename()
{
    return appFilename;
}

std::vector<std::shared_ptr<App>> &AppManager::getAppsList()
{
    return apps;
}

std::shared_ptr<App> AppManager::findApp(const std::string &name)
{
    if (name.empty())
    {
        return nullptr;
    }

    std::lock_guard<std::mutex> lock(apps_mutex);

    if (std::isdigit(static_cast<unsigned char>(name[0])))
    {
        try
        {
            std::size_t pos = 0;
            int idx = std::stoi(name, &pos);
            if (pos == name.size() && idx >= 0 &&
                static_cast<std::size_t>(idx) < apps.size())
            {
                return apps[static_cast<std::size_t>(idx)];
            }
        }
        catch (const std::exception &)
        {
            // Игнорируем и пробуем поиск по имени ниже
        }
    }

    for (auto &a : apps)
    {
        if (a->name() == name)
            return a;
    }
    return nullptr;
}

std::shared_ptr<App> AppManager::getApp(size_t index)
{
    std::lock_guard<std::mutex> lock(apps_mutex);
    if (index < apps.size())
        return apps[index];

    return nullptr;
}

const std::string &AppManager::getDeviceDescFilename() const
{
    return settingsFilename;
}

const std::string &AppManager::getDeviceRuntimeFilename() const
{
    return runtimeSettingsFilename;
}

std::list<uint8_t> &AppManager::errors()
{
    return errorList;
}

std::vector<std::string> &AppManager::getSystemLogPaths()
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
    for (const auto &app : apps)
    {
        auto logPaths = app->logPaths();
        if (!logPaths.empty())
        {
            for (const auto &path : logPaths)
                paths.push_back(path);
        }
    }

    for (const auto &path : paths)
    {
        std::ifstream f(path);
        if (f.is_open())
        {
            /*std::string s(std::istreambuf_iterator<char>{f}, {});
            size_t fileSize = s.size();
            size_t packedSize = fileSize;
            uLongf ulf_packedSize = fileSize;
            std::vector<uint8_t> output(fileSize);
            if (compress(output.data(), &ulf_packedSize, (Bytef *)s.data(),
                         fileSize) == Z_OK)
            {
                std::vector<uint8_t> packed(4);
                *(uint32_t *)(packed.data()) = bswap_32((uint32_t)packedSize);
                packed.insert(packed.end(), output.begin(),
                              output.begin() + packedSize);
                data.push_back({path, packed});
            }*/
        }
    }
    return data;
}

void AppManager::reload_config()
{
    closeApps();
    loadConfigFile();
    runApps();
}

void AppManager::on_child_finished(pid_t pid)
{
    auto app = get_app_by_pid(pid);
    app->on_child_finished();
}

std::shared_ptr<App> AppManager::get_app_by_pid(pid_t pid)
{
    std::lock_guard<std::mutex> lock(apps_mutex);
    for (auto &a : apps)
    {
        if (a->pid() == pid)
            return a;
    }
    return nullptr;
}

void AppManager::addApp(const std::string &name, const std::string &command,
                        App::RestartMode mode)
{
    std::lock_guard<std::mutex> lock(apps_mutex);
    apps.emplace_back(std::make_shared<App>(
        apps.size(), name, command, mode, std::vector<LinkedFile>{}, ""));
}

void AppManager::removeApp(size_t index)
{
    std::lock_guard<std::mutex> lock(apps_mutex);
    if (index < apps.size())
    {
        apps[index]->stop();
        apps.erase(apps.begin() + index);
    }
}

nos::trent AppManager::toJson() const
{
    std::lock_guard<std::mutex> lock(apps_mutex);
    nos::trent root;
    for (size_t i = 0; i < apps.size(); i++)
    {
        root["apps"][(int)i] = apps[i]->toTrent();
    }
    for (size_t i = 0; i < systemLogPaths.size(); i++)
    {
        root["sys_logs"][(int)i] = systemLogPaths[i];
    }
    return root;
}

void AppManager::saveConfig()
{
    std::ofstream file(appFilename);
    file << nos::json::to_string(toJson());
    file.close();
    nos::println("Config saved to", appFilename);
}

nos::trent AppManager::getAppsState() const
{
    std::lock_guard<std::mutex> lock(apps_mutex);
    nos::trent tr;
    for (size_t i = 0; i < apps.size(); i++)
    {
        tr["apps"][(int)i]["name"] = apps[i]->name();
        tr["apps"][(int)i]["state"] = apps[i]->status_string();
        tr["apps"][(int)i]["pid"] = apps[i]->pid();
    }
    return tr;
}

nos::trent AppManager::getAppsFullState() const
{
    std::lock_guard<std::mutex> lock(apps_mutex);
    nos::trent tr;
    for (size_t i = 0; i < apps.size(); i++)
    {
        tr["apps"][(int)i]["name"] = apps[i]->name();
        tr["apps"][(int)i]["state"] = apps[i]->status_string();
        tr["apps"][(int)i]["pid"] = apps[i]->pid();
        tr["apps"][(int)i]["command"] = apps[i]->command();
    }
    return tr;
}
