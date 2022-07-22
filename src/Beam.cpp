#include <Beam.h>
#include <igris/trent/json.h>
#include <igris/trent/json_print.h>

std::map<std::string, igris::trent(Beam::*)(const igris::trent&)> Beam::method_map = {
    { "getsettings", &Beam::getsettings },
};


Beam::Beam()
{
    recv_socket.init();
    recv_socket.bind(RECVPORT);

    send_socket.init();
    send_socket.bind(SENDPORT);
    send_socket.allow_broadcast();
}

void Beam::start()
{
    beam_thread = std::thread(&Beam::beam_thread_func, this);
}

void Beam::beam_thread_func()
{
    nos::println("starting beam thread");
    for (;;)
    {
        auto [data, ip, port] = recv_socket.recvfrom();
        nos::println("NewBeamMessage:", data);

        try 
        {
            igris::trent tr = igris::json::parse(data);
            on_new_command(tr, ip, port);
        } 
        catch (const std::exception& e)
        {
            nos::println("Beam error:", e.what());
        }
    }
}

igris::trent Beam::getsettings(const igris::trent& tr) 
{
    return tr;
}

void Beam::on_new_command(const igris::trent& tr, std::string ip, int port) 
{
    if (!tr.have("cmd")) 
    {
        throw std::runtime_error("cmd field is required");
    }
    auto cmd = tr["cmd"].as_string();

    auto rettr = (this->*method_map[cmd])(tr);
    auto json = igris::json::to_string(rettr, false);
    send_socket.sendto(json.data(), json.size(), ip, port);
}