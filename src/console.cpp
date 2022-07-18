#include <AppManager.h>
#include <console.h>
#include <igris/trent/json.h>
#include <igris/trent/json_print.h>
#include <igris/util/base64.h>
#include <iostream>
#include <modes.h>
#include <nos/inet/tcp_server.h>
#include <nos/inet/tcp_socket.h>
#include <nos/io/buffered_file.h>
#include <nos/shell/argv.h>
#include <nos/shell/executor.h>
#include <thread>

const int API_VERSION = 100;
extern AppManager *appManager;
extern void stop_all_threads();

int exit(const nos::argv &args, nos::ostream &out)
{
    (void)args;
    (void)out;
    appManager->closeApps();
    quick_exit(0);
    return 0;
}

int hello(const nos::argv &args, nos::ostream &out)
{
    (void)args;
    (void)out;
    out.println("Hello");
    return 0;
}

int start_application(const nos::argv &args, nos::ostream &out)
{
    if (args.size() < 2)
    {
        out.println("Usage: start_application <app_name>");
        return -1;
    }

    auto *app = appManager->findApp(args[1].to_string());
    if (app)
    {
        app->start();
        out.println("Started application: " + app->name());
    }
    else
    {
        out.println("Application not found: " + args[1].to_string());
        return -1;
    }

    return 0;
}

int stop_application(const nos::argv &args, nos::ostream &out)
{
    if (args.size() < 2)
    {
        out.println("Usage: stop_application <app_name>");
        return -1;
    }

    auto *app = appManager->findApp(args[1].to_string());
    if (app)
    {
        app->stop();
        out.println("Stopped application: " + app->name());
    }
    else
    {
        out.println("Application not found: " + args[1].to_string());
        return -1;
    }

    return 0;
}

int stop_id_application(const nos::argv &args, nos::ostream &out)
{
    if (args.size() < 2)
    {
        out.println("Usage: stop_id_application <app_id>");
        return -1;
    }

    auto *app = appManager->getApp(args[1].to_int());
    if (app)
    {
        app->stop();
        out.println("Stopped application: " + app->name());
    }
    else
    {
        out.println("Application not found: " + args[1].to_string());
        return -1;
    }

    return 0;
}

int start_id_application(const nos::argv &args, nos::ostream &out)
{
    if (args.size() < 2)
    {
        out.println("Usage: start_id_application <app_id>");
        return -1;
    }

    auto *app = appManager->getApp(args[1].to_int());
    if (app)
    {
        app->start();
        out.println("Started application: " + app->name());
    }
    else
    {
        out.println("Application not found: " + args[1].to_string());
        return -1;
    }

    return 0;
}

int list_of_applications(const nos::argv &args, nos::ostream &out)
{
    (void)args;
    auto &apps = appManager->applications();
    for (auto &app : apps)
        nos::fprintln_to(out, "{} : {} : {}", app.name(), app.status_string(),
                         app.pid());
    return 0;
}

int stop_all_applications(const nos::argv &args, nos::ostream &out)
{
    (void)args;
    (void)out;
    appManager->closeApps();
    return 0;
}

int start_all_applications(const nos::argv &args, nos::ostream &out)
{
    (void)args;
    (void)out;
    appManager->runApps();
    return 0;
}

int restart_all_applications(const nos::argv &args, nos::ostream &out)
{
    (void)args;
    (void)out;
    appManager->restartApps();
    return 0;
}

int show_application_stdout(const nos::argv &args, nos::ostream &out)
{
    if (args.size() < 2)
    {
        out.println("Usage: show_application_stdout <app_name>");
        return -1;
    }

    auto *app = appManager->findApp(args[1].to_string());
    if (app)
    {
        const std::string &stdout_string = app->show_stdout();
        nos::println_to(out, stdout_string);
    }
    else
    {
        nos::println_to(out, "Application not found: " + args[1].to_string());
        return -1;
    }

    return 0;
}

int show_application_stdout_base64(const nos::argv &args, nos::ostream &out)
{
    nos::println("show_application_stdout: {}", args[1]);
    if (args.size() < 2)
    {
        out.println("Usage: show_application_stdout <app_name>");
        return -1;
    }

    auto *app = appManager->findApp(args[1].to_string());
    if (app)
    {
        const std::string &stdout_string = app->show_stdout();
        nos::println_to(out, igris::base64_encode(stdout_string));
    }
    else
    {
        nos::println_to(out, "Application not found: " + args[1].to_string());
        return -1;
    }

    return 0;
}

int send_spam(const nos::argv &args, nos::ostream &out)
{
    (void)out;
    if (args.size() < 2)
    {
        out.println("Usage: send_spam <word>");
        return -1;
    }

    appManager->send_spam(igris::base64_encode(args[1].to_string() + "\n"));

    return 0;
}

int api_version(const nos::argv &args, nos::ostream &out)
{
    (void)args;
    out.println(API_VERSION);
    return 0;
}

int app_linked_files(const nos::argv &args, nos::ostream &out)
{
    if (args.size() < 2)
    {
        out.println("Usage: linkeds <app_name>");
        return -1;
    }

    auto *app = appManager->findApp(args[1].to_string());
    const auto &linked_files = app->linked_files();

    igris::trent tr(igris::trent::type::list);
    for (auto &file : linked_files)
    {
        tr.push_back(file.to_trent());
    }
    out.print(igris::json::to_string(tr));
    return 0;
}

int app_linked_files_b64(const nos::argv &args, nos::ostream &out)
{
    if (args.size() < 2)
    {
        out.println("Usage: linkeds_b64 <app_name>");
        return -1;
    }

    auto *app = appManager->findApp(args[1].to_string());
    const auto &linked_files = app->linked_files();

    igris::trent tr(igris::trent::type::list);
    for (auto &file : linked_files)
    {
        tr.push_back(file.to_trent());
    }
    out.println(igris::base64_encode(igris::json::to_string(tr)));
    return 0;
}

int read_linked_file(const nos::argv &args, nos::ostream &out)
{
    if (args.size() < 3)
    {
        out.println("Usage: read_linked <app_name> <file_name>");
        return -1;
    }

    auto *app = appManager->findApp(args[1].to_string());
    if (app)
    {
        const auto &linked_files = app->linked_files();
        for (auto &file : linked_files)
        {
            if (file.name == args[2].to_string())
            {
                if (!nos::osutil::is_accessible(file.path))
                {
                    out.println("Can`t read file (is it exists?): " +
                                file.path);
                    return -1;
                }

                nos::buffered_file f(file.path, "r");
                auto s = f.readall();
                out.println(s);
                return 0;
            }
        }
    }

    out.println("File not found");
    return -1;
}

int read_linked_file_b64(const nos::argv &args, nos::ostream &out)
{
    if (args.size() < 3)
    {
        out.println("Usage: read_linked_b64 <app_name> <file_name>");
        return -1;
    }

    auto *app = appManager->findApp(args[1].to_string());
    if (app)
    {
        const auto &linked_files = app->linked_files();
        for (auto &file : linked_files)
        {
            if (file.name == args[2].to_string())
            {
                if (!nos::osutil::is_accessible(file.path))
                {
                    out.println(igris::base64_encode(
                        "Can`t read file (is it exists?): " + file.path));
                    return -1;
                }

                nos::buffered_file f(file.path, "r");
                auto s = f.readall();
                auto b = igris::base64_encode(s);
                out.println(b);
                return 0;
            }
        }
    }

    out.println("File not found");
    return -1;
}

int apps_config_b64(const nos::argv &args, nos::ostream &out)
{
    (void)args;
    auto path = appManager->getAppConfigFilename();
    auto file = nos::buffered_file(path.c_str(), "r");
    auto text = nos::readall_from(file);
    out.println(igris::base64_encode(text));
    file.close();
    return 0;
}

int set_apps_config_b64(const nos::argv &args, nos::ostream &out)
{
    if (args.size() < 2)
    {
        out.println("Usage: set_apps_config_b64 <config_b64>");
        return -1;
    }
    // nos::println("set_apps_config_b64: {}", args[1].to_string());

    auto decoded = igris::base64_decode(args[1].to_string());
    auto path = appManager->getAppConfigFilename();
    auto file = nos::buffered_file(path.c_str(), "w");
    nos::print_to(file, decoded);
    file.close();
    appManager->reload_config();
    return 0;
}

int reload_config(const nos::argv &args, nos::ostream &out)
{
    (void)args;
    (void)out;
    appManager->reload_config();
    return 0;
}

std::thread server_thread;
nos::executor executor(std::vector<nos::command>{
    nos::command("hello", "baba is you", &hello),
    nos::command("q", "exit", &exit),
    nos::command("exit", "exit", &exit),
    nos::command("reload", "reload", &reload_config),
    nos::command("list", "list of applications", &list_of_applications),
    nos::command("stop", "stop application", &stop_application),
    nos::command("start", "start application", &start_application),
    nos::command("stop_id", "stop application", &stop_id_application),
    nos::command("start_id", "start application", &start_id_application),
    nos::command("stop_all", "stop all applications", &stop_all_applications),
    nos::command("start_all", "start all applications",
                 &start_all_applications),
    nos::command("restart_all", "restart all applications",
                 &restart_all_applications),
    nos::command("log", "show application stdout", &show_application_stdout),
    nos::command("log_base64", "show application stdout",
                 &show_application_stdout_base64),
    nos::command("spam", "send spam", &send_spam),
    nos::command("api_version", "api version", &api_version),
    nos::command("linkeds", "linked files", &app_linked_files),
    nos::command("linkeds_b64", "linked files", &app_linked_files_b64),
    nos::command("apps_config_b64", "get apps config", &apps_config_b64),
    nos::command("set_apps_config_b64", "set apps config",
                 &set_apps_config_b64),
    nos::command("read_linked", "read linked file", &read_linked_file),
    nos::command("read_linked_b64", "read linked file",
                 &read_linked_file_b64)});

std::string execute_tokens(nos::tokens &tokens)
{
    if (VERBOSE)
    {
        nos::print("New request: ");
        nos::print_list(tokens);
        nos::println();
    }

    nos::string_buffer sb;
    if (tokens.size() == 0)
        return "";
    executor.execute(tokens, sb);
    return sb.str();
}

void client_spin(nos::inet::tcp_socket client)
{
    try
    {
        while (true)
        {
            std::string line = nos::readline_from(client);
            if (line.size() == 0)
                return;

            nos::tokens tokens(line);
            auto sb = execute_tokens(tokens);
            if (sb.size() == 0)
                continue;
            client.write(sb.data(), sb.size());
        }
    }
    catch (const std::exception &ex)
    {
        nos::println("client finished by exception", ex.what());
    }
}

void server_spin(int port)
{
    nos::inet::tcp_server server;
    server.init();
    server.reusing(true);
    server.bind("0.0.0.0", port);
    server.listen();

    while (true)
    {
        nos::println("Waiting for client...");
        nos::inet::tcp_socket client = server.accept();
        nos::println("Client connected");
        std::thread client_thread(client_spin, client);
        client_thread.detach();
    }
}

void start_tcp_console(int tcp_console_port)
{
    server_thread = std::thread(server_spin, tcp_console_port);
}

int userIOThreadHandler()
{
    std::string str;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    while (1)
    {
        std::cout << "$ ";
        getline(std::cin, str);
        nos::tokens tokens(str);
        auto sb = execute_tokens(tokens);
        if (sb.size() == 0)
            continue;
        std::cout << sb;
    }
    return 0;
}

void start_stdstream_console()
{
    std::thread userIOThread(userIOThreadHandler);
    userIOThread.detach();
}
