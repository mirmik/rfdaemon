
#include <console.h>
#include <nos/inet/tcp_socket.h>
#include <nos/inet/tcp_server.h>
#include <nos/shell/argv.h>
#include <nos/shell/executor.h>
#include <thread>
#include <iostream>
#include <AppManager.h>

extern AppManager* appManager;

int hello(const nos::argv& args, nos::ostream& out) 
{
    out.println("Hello");
    return 0;
}

int start_application(const nos::argv& args, nos::ostream& out) 
{
    if (args.size() < 2)
    {
        out.println("Usage: start_application <app_name>");
        return -1;
    }

    auto* app = appManager->findApp(args[1].to_string());
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

int stop_application(const nos::argv& args, nos::ostream& out) 
{
    if (args.size() < 2)
    {
        out.println("Usage: stop_application <app_name>");
        return -1;
    }

    auto* app = appManager->findApp(args[1].to_string());
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

int stop_id_application(const nos::argv& args, nos::ostream& out) 
{
    if (args.size() < 2)
    {
        out.println("Usage: stop_id_application <app_id>");
        return -1;
    }

    nos::println("stop_id_application: {}", args[0]);
    nos::println("stop_id_application: {}", args[1]);
    auto* app = appManager->getApp(args[1].to_int());
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

int start_id_application(const nos::argv& args, nos::ostream& out) 
{
    if (args.size() < 2)
    {
        out.println("Usage: start_id_application <app_id>");
        return -1;
    }

    nos::println("start_id_application: {}", args[0]);
    nos::println("start_id_application: {}", args[1]);
    auto* app = appManager->getApp(args[1].to_int());
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

int list_of_applications(const nos::argv& args, nos::ostream& out) 
{
    auto& apps = appManager->applications();
    for (auto& app : apps)
        nos::fprintln_to(out, "{} : {} : {}", app.name(), app.status_string(), app.pid());
    return 0;
}

std::thread server_thread;
nos::executor executor(std::vector<nos::command>{
    nos::command("hello", "hello is hello", &hello),
    nos::command("list", "list of applications", &list_of_applications),
    nos::command("stop", "stop application", &stop_application),
    nos::command("start", "start application", &start_application),
    nos::command("stop_id", "stop application", &stop_id_application),
    nos::command("start_id", "start application", &start_id_application),
});

void client_spin(nos::inet::tcp_socket client) 
{
    char buf[1024];
    while(true) 
    {
        memset(buf, 0, sizeof(buf));
        client.read(buf, sizeof(buf));
        if (buf[0] == '\0')
            break;
        nos::tokens tokens(buf);
        nos::string_buffer sb;
        executor.execute(tokens, sb);
        client.write(sb.str().data(), sb.str().size());
    }
}

void server_spin(int port) 
{
    nos::inet::tcp_server server("0.0.0.0", port);

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
    while (1)
    {
        getline(std::cin, str);
        nos::tokens tokens(str);
        nos::string_buffer sb;
        executor.execute(tokens, sb);
        std::cout << sb.str();
        /*if (userInput.find("get ") == 0)
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
        else if (userInput.find("restart") == 0)
            appManager->restartApps();
        else if (userInput.find("stop") == 0)
            appManager->closeApps();
        else if (userInput.find("connstatus") == 0)
        {
            if (srv)
                cout << (srv->clientConnected() ? ("Connected to " + srv->getClientInfo()) : "Disconnected") << endl;
        }
        else if (userInput.find("quit") == 0)
            exitHandler(0);
        else
            cout << "Wrong command." << endl;*/       
    }
    return 0;
}

void start_istream_console() 
{
    std::thread userIOThread(userIOThreadHandler);
    userIOThread.detach();
}
