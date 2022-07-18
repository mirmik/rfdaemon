#include <Beam.h>

Beam::Beam(uint16_t sendport, uint16_t recvport)
{
    this->recvport = recvport;
    beam_socket.init();
    beam_socket.bind(sendport);
    beam_socket.allow_broadcast();
}

void Beam::start()
{
    beam_thread = std::thread(&Beam::beam_thread_func, this);
}

void Beam::beam_thread_func()
{
    for (;;)
    {
        beam_socket.send_broadcast("Hello, world!", 12, recvport);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}