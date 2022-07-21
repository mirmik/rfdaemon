#ifndef BEAM_H
#define BEAM_H

#include <nos/inet/udp_socket.h>
#include <thread>

class Beam
{
    nos::inet::udp_broadcast_socket recv_socket;
    nos::inet::udp_broadcast_socket send_socket;
    std::thread beam_thread;

    static constexpr int RECVPORT = 9835;
    static constexpr int SENDPORT = 9836;

public:
    Beam();
    void start();

private:
    void beam_thread_func();
};

#endif