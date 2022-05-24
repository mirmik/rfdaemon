
#include <console.h>
#include <nos/inet/tcp_socket.h>
#include <nos/inet/tcp_server.h>
#include <nos/shell/argv.h>
#include <nos/shell/executor.h>
#include <thread>

int hello(const nos::argv& args, nos::ostream& out) 
{
    out.println("Hello");
    return 0;
}

std::thread server_thread;
nos::executor executor(std::vector<nos::command>{
    nos::command("hello", "hello is hello", &hello)
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
        if (tokens.size() == 0)
            continue;
        if (tokens[0] == "exit")
            break;
        if (tokens[0] == "help")
        {
            nos::println("exit - exit from client");
            nos::println("help - show this help");
            continue;
        }
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