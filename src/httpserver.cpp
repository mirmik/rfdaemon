//#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <AppManager.h>
#include <chrono>
#include <httplib.h>
#include <ircc/ircc.h>
#include <memory>
#include <nos/fprint.h>
#include <igris/trent/json_print.h>
#include <nos/io/sstream.h>
#include <thread>

extern std::unique_ptr<AppManager> appManager;
using namespace nos::argument_literal;
using namespace std::chrono_literals;

void bind_static_html_resource(httplib::Server &srv, std::string path,
                               std::string resource, std::string content_type)
{
    srv.Get(path, [&srv, path, resource, content_type](const httplib::Request &,
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

void start_httpserver()
{
    std::thread([]() {
        httplib::Server server;
        bind_static_html_resource(server, "/", "/web/index.html", "text/html");
        bind_static_html_resource(server, "/index.html", "/web/index.html", "text/html");
        bind_static_html_resource(server, "/functions.js", "/web/functions.js", "text/javascript");
        bind_static_html_resource(server, "/style.css", "/web/style.css", "text/css");

        server.Get("/apps_state.json", [&server](const httplib::Request &,
                                                 httplib::Response &res) {
            auto &apps = appManager->applications();
            igris::trent tr;
            for (size_t i = 0; i < apps.size(); i++)
            {
                tr["apps"][i]["name"] = apps[i].name();
                tr["apps"][i]["state"] = apps[i].status_string();
                tr["apps"][i]["pid"] = apps[i].pid();
            }
            res.set_content(igris::json::to_string(tr), "application/json");
        });

        server.Get("/apps_full_state.json", [&server](const httplib::Request &,
                                                 httplib::Response &res) {
            auto &apps = appManager->applications();
            igris::trent tr;
            for (size_t i = 0; i < apps.size(); i++)
            {
                tr["apps"][i]["name"] = apps[i].name();
                tr["apps"][i]["state"] = apps[i].status_string();
                tr["apps"][i]["pid"] = apps[i].pid();
                tr["apps"][i]["command"] = apps[i].command();
            }
            res.set_content(igris::json::to_string(tr), "application/json");
        });

        server.Get("/stop_all.action", [&server](const httplib::Request &,
                                                 httplib::Response &res) {
            std::cout << "stop_all" << std::endl;
            appManager->stop_all();
            res.set_content("{\"status\":\"ok\"}", "application/json");
        });

        server.Get("/start_all.action", [&server](const httplib::Request &,
                                                  httplib::Response &res) {
            std::cout << "start_all" << std::endl;
            appManager->start_all();
            res.set_content("{\"status\":\"ok\"}", "application/json");
        });

        server.Get("/stop.action", [&server](const httplib::Request &req,
                                             httplib::Response &res) {
            auto index = std::stoi(req.get_param_value("index"));
            std::cout << "stop " << index << std::endl;
            appManager->applications()[index].stop();
            res.set_content("{\"status\":\"ok\"}", "application/json");
        });

        server.Get("/start.action", [&server](const httplib::Request &req,
                                              httplib::Response &res) {
            auto index = std::stoi(req.get_param_value("index"));
            std::cout << "start " << index << std::endl;
            appManager->applications()[index].start();
            res.set_content("{\"status\":\"ok\"}", "application/json");
        });

        server.Get("/restart.action", [&server](const httplib::Request &req,
                                                httplib::Response &res) {
            auto index = std::stoi(req.get_param_value("index"));
            std::cout << "restart " << index << std::endl;
            appManager->applications()[index].restart();
            res.set_content("{\"status\":\"ok\"}", "application/json");
        });

        server.set_error_handler([](const auto &req, auto &res) {
            auto fmt = "<p>Error Path:%s Status: <span "
                       "style='color:red;'>%d</span></p>";
            char buf[BUFSIZ];
            nos::fprintln("Error: request:{} status:{}", req.path, res.status);
            snprintf(buf, sizeof(buf), fmt, req.path, res.status);
            res.set_content(buf, "text/html");
        });

        int port = 9000;
        nos::fprint("Starting HTTP server on port {}\n", port);
        server.listen("0.0.0.0", port);
    }).detach();
}