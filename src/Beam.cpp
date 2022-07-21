#include <Beam.h>

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
        nos::println(data);
    }
}