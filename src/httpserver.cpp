//#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <AppManager.h>
#include <chrono>
#include <httplib_static.h>
#include <ircc/ircc.h>
#include <memory>
#include <nos/fprint.h>
#include <nos/io/sstream.h>
#include <nos/trent/json.h>
#include <nos/trent/json_print.h>
#include <thread>

extern std::unique_ptr<AppManager> appManager;
using namespace nos::argument_literal;
using namespace std::chrono_literals;

void bind_static_html_resource(httplib::Server &srv, std::string path,
                               std::string resource, std::string content_type)
{
    srv.Get(path, [path, resource, content_type](const httplib::Request &,
                                                 httplib::Response &res) {
        std::string text = ircc_string(resource.c_str());
        res.set_content(text, content_type);
    });
}

/*void bind_all_resources_with_prefix(httplib::Server &srv, std::string path,
                                    std::string prefix)
{
    auto resources = ircc_keys();
    for (auto &resource : resources)
    {
        if (resource.find(prefix) == 0)
        {
            auto resource_without_prefix = resource.substr(prefix.length());
            bind_static_html_resource(srv, path + resource_without_prefix,
                                      resource);
        }
    }
}*/

static std::thread httpserver_thread;
static std::unique_ptr<httplib::Server> http_server_ptr;

void stop_httpserver()
{
    if (http_server_ptr)
    {
        http_server_ptr->stop();
    }
    if (httpserver_thread.joinable())
    {
        httpserver_thread.join();
    }
    http_server_ptr.reset();
}

void start_httpserver(const std::string &host, uint16_t port)
{
    httpserver_thread = std::thread([host, port]() {
        http_server_ptr = std::make_unique<httplib::Server>();
        auto &server = *http_server_ptr;
        bind_static_html_resource(server, "/", "/web/index.html", "text/html");
        bind_static_html_resource(server, "/index.html", "/web/index.html",
                                  "text/html");
        bind_static_html_resource(server, "/functions.js", "/web/functions.js",
                                  "text/javascript");
        bind_static_html_resource(server, "/style.css", "/web/style.css",
                                  "text/css");

        server.Get("/apps_state.json", [](const httplib::Request &,
                                          httplib::Response &res) {
            auto tr = appManager->getAppsState();
            res.set_content(nos::json::to_string(tr), "application/json");
        });

        server.Get("/apps_full_state.json", [](const httplib::Request &,
                                               httplib::Response &res) {
            auto tr = appManager->getAppsFullState();
            res.set_content(nos::json::to_string(tr), "application/json");
        });

        server.Get("/stop_all.action", [](const httplib::Request &,
                                          httplib::Response &res) {
            std::cout << "stop_all" << std::endl;
            appManager->stop_all();
            res.set_content("{\"status\":\"ok\"}", "application/json");
        });

        server.Get("/start_all.action", [](const httplib::Request &,
                                           httplib::Response &res) {
            std::cout << "start_all" << std::endl;
            appManager->start_all();
            res.set_content("{\"status\":\"ok\"}", "application/json");
        });

        server.Get("/stop.action", [](const httplib::Request &req,
                                      httplib::Response &res) {
            auto index = std::stoi(req.get_param_value("index"));
            std::cout << "stop " << index << std::endl;
            auto app = appManager->getApp(index);
            if (!app) {
                res.set_content("{\"error\":\"not found\"}", "application/json");
                return;
            }
            app->stop();
            res.set_content("{\"status\":\"ok\"}", "application/json");
        });

        server.Get("/start.action", [](const httplib::Request &req,
                                       httplib::Response &res) {
            auto index = std::stoi(req.get_param_value("index"));
            std::cout << "start " << index << std::endl;
            auto app = appManager->getApp(index);
            if (!app) {
                res.set_content("{\"error\":\"not found\"}", "application/json");
                return;
            }
            app->start();
            res.set_content("{\"status\":\"ok\"}", "application/json");
        });

        server.Get("/restart.action", [](const httplib::Request &req,
                                         httplib::Response &res) {
            auto index = std::stoi(req.get_param_value("index"));
            std::cout << "restart " << index << std::endl;
            auto app = appManager->getApp(index);
            if (!app) {
                res.set_content("{\"error\":\"not found\"}", "application/json");
                return;
            }
            app->restart();
            res.set_content("{\"status\":\"ok\"}", "application/json");
        });

        server.Get("/get_logs.action", [](const httplib::Request &req,
                                          httplib::Response &res) {
            auto index = std::stoi(req.get_param_value("index"));
            std::cout << "get_logs " << index << std::endl;
            auto app = appManager->getApp(index);
            if (!app) {
                res.set_content("{\"error\":\"not found\"}", "application/json");
                return;
            }
            auto logs = httplib::detail::base64_encode(app->show_stdout());
            nos::trent tr;
            tr["stdout"] = logs;
            res.set_content(nos::json::to_string(tr), "application/json");
        });

        // Update app parameters
        server.Post("/app_update.action",
                    [](const httplib::Request &req, httplib::Response &res) {
                        std::cout << "app_update" << std::endl;
                        auto body = nos::json::parse(req.body);
                        int index = static_cast<int>(body["index"].as_numer());
                        auto app = appManager->getApp(index);
                        if (!app)
                        {
                            res.set_content("{\"error\":\"not found\"}",
                                            "application/json");
                            return;
                        }
                        if (body.contains("name"))
                            app->setName(body["name"].as_string());
                        if (body.contains("command"))
                            app->setCommand(body["command"].as_string());
                        if (body.contains("restart"))
                        {
                            auto mode =
                                body["restart"].as_string() == "always"
                                    ? App::RestartMode::ALWAYS
                                    : App::RestartMode::ONCE;
                            app->setRestartMode(mode);
                        }
                        res.set_content("{\"status\":\"ok\"}",
                                        "application/json");
                    });

        // Add new app
        server.Post(
            "/app_add.action",
            [](const httplib::Request &req, httplib::Response &res) {
                std::cout << "app_add" << std::endl;
                auto body = nos::json::parse(req.body);
                std::string name = body["name"].as_string();
                std::string command = body["command"].as_string();
                auto mode = body["restart"].as_string() == "always"
                                ? App::RestartMode::ALWAYS
                                : App::RestartMode::ONCE;
                appManager->addApp(name, command, mode);
                res.set_content("{\"status\":\"ok\"}", "application/json");
            });

        // Delete app
        server.Get("/app_delete.action", [](const httplib::Request &req,
                                            httplib::Response &res) {
            int index = std::stoi(req.get_param_value("index"));
            std::cout << "app_delete " << index << std::endl;
            if (index < 0 || static_cast<size_t>(index) >= appManager->getAppCount()) {
                res.set_content("{\"error\":\"not found\"}", "application/json");
                return;
            }
            appManager->removeApp(index);
            res.set_content("{\"status\":\"ok\"}", "application/json");
        });

        // Save config to disk
        server.Get("/save_config.action",
                   [](const httplib::Request &, httplib::Response &res) {
                       std::cout << "save_config" << std::endl;
                       appManager->saveConfig();
                       res.set_content("{\"status\":\"ok\"}",
                                       "application/json");
                   });

        server.set_error_handler([](const auto &req, auto &res) {
            auto fmt = "<p>Error Path:%s Status: <span "
                       "style='color:red;'>%d</span></p>";
            char buf[BUFSIZ];
            nos::fprintln("Error: request:{} status:{}", req.path, res.status);
            snprintf(buf, sizeof(buf), fmt, req.path.c_str(), res.status);
            res.set_content(buf, "text/html");
        });

        nos::fprint("HTTP server listening on {}:{}\n", host, port);
        if (!server.listen(host, port))
        {
            nos::fprintln("ERROR: Failed to start HTTP server on {}:{}", host, port);
        }
    });
}