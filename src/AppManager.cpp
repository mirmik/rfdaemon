#include "AppManager.h"
#include <byteswap.h>
#include <cctype>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <mutex>
#include <nos/trent/json.h>
#include <nos/trent/json_print.h>
#include <set>
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

            apps.emplace_back(
                std::make_shared<App>(apps.size(), name, cmd, restartMode, linked_files,
                              user));
            auto &app = apps.back();

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
    nos::fprintln("[runApps] Called from thread {}, starting {} apps", std::this_thread::get_id(), apps.size());
    std::lock_guard<std::mutex> lock(apps_mutex);

    // First, sync all service files
    bool need_reload = false;
    for (size_t i = 0; i < apps.size(); i++)
    {
        if (apps[i]->sync_service_file())
            need_reload = true;
    }

    // Reload systemd if any service files changed
    if (need_reload)
    {
        App::daemon_reload();
    }

    // Now start all apps
    for (size_t i = 0; i < apps.size(); i++)
    {
        auto &a = apps[i];
        if (a->stopped())
        {
            nos::fprintln("[runApps] Starting app {} '{}'...", i, a->name());
            // start() won't call sync_service_file again if already synced
            std::string cmd = "systemctl start " + a->service_name();
            int ret = system(cmd.c_str());
            if (ret == 0)
            {
                a->isStopped = false;
                nos::fprintln("[runApps] App {} '{}' started", i, a->name());
            }
            else
            {
                nos::fprintln("[runApps] App {} '{}' failed to start", i, a->name());
            }
        }
    }
    nos::println("[runApps] All apps started");
}

void AppManager::closeApps()
{
    nos::fprintln("[closeApps] Called from thread {}, acquiring mutex...", std::this_thread::get_id());
    std::lock_guard<std::mutex> lock(apps_mutex);
    nos::fprintln("[closeApps] Mutex acquired, stopping {} apps", apps.size());
    for (size_t i = 0; i < apps.size(); i++)
    {
        nos::fprintln("[closeApps] Stopping app {} '{}'...", i, apps[i]->name());
        if (!apps[i]->stopped())
            apps[i]->stop();
        nos::fprintln("[closeApps] App {} '{}' stopped", i, apps[i]->name());
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
    cleanup_orphaned_services();
    runApps();
}

void AppManager::cleanup_orphaned_services()
{
    nos::println("[cleanup_orphaned_services] Scanning for orphaned services...");

    // Build set of current service names
    std::set<std::string> current_services;
    {
        std::lock_guard<std::mutex> lock(apps_mutex);
        for (const auto &app : apps)
        {
            current_services.insert(app->service_name());
        }
    }

    // Scan /etc/systemd/system/ for rfd-*.service files
    const char *systemd_dir = "/etc/systemd/system";
    DIR *dir = opendir(systemd_dir);
    if (!dir)
    {
        nos::fprintln("[cleanup_orphaned_services] Cannot open {}", systemd_dir);
        return;
    }

    std::vector<std::string> orphaned;
    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr)
    {
        std::string name = entry->d_name;
        // Check if it's an rfd-*.service file
        if (name.size() > 4 && name.substr(0, 4) == "rfd-" &&
            name.size() > 8 && name.substr(name.size() - 8) == ".service")
        {
            // Extract service name (without .service)
            std::string service_name = name.substr(0, name.size() - 8);
            if (current_services.find(service_name) == current_services.end())
            {
                orphaned.push_back(service_name);
            }
        }
    }
    closedir(dir);

    // Stop and remove orphaned services
    bool need_reload = false;
    for (const auto &service : orphaned)
    {
        nos::fprintln("[cleanup_orphaned_services] Removing orphaned service: {}",
                      service);

        // Stop the service
        std::string stop_cmd = "systemctl stop " + service + " 2>/dev/null";
        system(stop_cmd.c_str());

        // Disable the service
        std::string disable_cmd = "systemctl disable " + service + " 2>/dev/null";
        system(disable_cmd.c_str());

        // Remove the service file
        std::string path = std::string(systemd_dir) + "/" + service + ".service";
        if (unlink(path.c_str()) == 0)
        {
            nos::fprintln("[cleanup_orphaned_services] Removed {}", path);
            need_reload = true;
        }
    }

    if (need_reload)
    {
        App::daemon_reload();
    }

    nos::fprintln("[cleanup_orphaned_services] Done, removed {} orphaned services",
                  orphaned.size());
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
        auto &app = apps[index];

        // Stop the systemd service
        app->stop();

        // Remove the .service file
        std::string service_path = app->service_path();
        std::string service_name = app->service_name();

        // Disable the service
        std::string disable_cmd = "systemctl disable " + service_name + " 2>/dev/null";
        system(disable_cmd.c_str());

        // Remove the service file
        if (unlink(service_path.c_str()) == 0)
        {
            nos::fprintln("[removeApp] Removed service file: {}", service_path);
            App::daemon_reload();
        }

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
