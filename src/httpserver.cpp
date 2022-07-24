//#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <AppManager.h>
#include <chrono>
#include <httplib.h>
#include <ircc/ircc.h>
#include <memory>
#include <nos/fprint.h>
#include <nos/io/sstream.h>
#include <thread>

extern std::unique_ptr<AppManager> appManager;
using namespace nos::argument_literal;
using namespace std::chrono_literals;

void bind_static_html_resource(httplib::Server &srv, std::string path,
                               std::string resource)
{
    srv.Get(
        path,
        [&srv, path, resource](const httplib::Request &, httplib::Response &res)
        {
            std::string text = ircc_string(resource.c_str());
            res.set_content(text, "text/html");
        });
}

void bind_all_resources_with_prefix(httplib::Server &srv, std::string path,
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
}

void start_httpserver()
{
    std::thread(
        []()
        {
            httplib::Server server;
            bind_static_html_resource(server, "/", "/web/index.html");
            bind_all_resources_with_prefix(server, "/", "/web/");

            server.Get(
                "/apps_state.json",
                [&server](const httplib::Request &, httplib::Response &res)
                {
                    auto &apps = appManager->applications();
                    nos::stringstream ss;
                    ss << "{\"apps\":[";
                    for (size_t i = 0; i < apps.size(); i++)
                    {
                        ss << "{";
                        ss << "\"name\":\"" << apps[i].name() << "\",";
                        ss << "\"state\":\"" << apps[i].status_string()
                           << "\",";
                        ss << "\"pid\":" << apps[i].pid();
                        ss << "}";
                        if (i < apps.size() - 1)
                            ss << ",";
                    }
                    ss << "]}";
                    res.set_content(ss.str(), "application/json");
                });

            server.Get(
                "/stop_all.action",
                [&server](const httplib::Request &, httplib::Response &res)
                {
                    std::cout << "stop_all" << std::endl;
                    appManager->stop_all();
                    res.set_content("{\"status\":\"ok\"}", "application/json");
                });

            server.Get(
                "/start_all.action",
                [&server](const httplib::Request &, httplib::Response &res)
                {
                    std::cout << "start_all" << std::endl;
                    appManager->start_all();
                    res.set_content("{\"status\":\"ok\"}", "application/json");
                });

            server.Get(
                "/stop.action",
                [&server](const httplib::Request &req, httplib::Response &res)
                {
                    auto index = std::stoi(req.get_param_value("index"));
                    std::cout << "stop " << index << std::endl;
                    appManager->applications()[index].stop();
                    res.set_content("{\"status\":\"ok\"}", "application/json");
                });

            server.Get(
                "/start.action",
                [&server](const httplib::Request &req, httplib::Response &res)
                {
                    auto index = std::stoi(req.get_param_value("index"));
                    std::cout << "start " << index << std::endl;
                    appManager->applications()[index].start();
                    res.set_content("{\"status\":\"ok\"}", "application/json");
                });

            server.listen("0.0.0.0", 9000);
        })
        .detach();
}