#ifndef BEAM_H
#define BEAM_H

#include <nos/inet/udp_socket.h>
#include <thread>

class Beam
{
    nos::inet::udp_broadcast_socket beam_socket;
    std::thread beam_thread;
    uint16_t recvport;

public:
    Beam(uint16_t sendport, uint16_t recvport);
    void start();

private:
    void beam_thread_func();
};

#endif