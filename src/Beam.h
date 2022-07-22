#ifndef BEAM_H
#define BEAM_H

#include <nos/inet/udp_socket.h>
#include <thread>
#include <map>
#include <igris/trent/trent.h>

class Beam
{
    static std::map<std::string, igris::trent(Beam::*)(const igris::trent&)> method_map;
    nos::inet::udp_broadcast_socket recv_socket;
    nos::inet::udp_broadcast_socket send_socket;
    std::thread beam_thread;

    static constexpr int RECVPORT = 9835;
    static constexpr int SENDPORT = 9836;

public:
    Beam();
    void start();
    void on_new_command(const igris::trent& tr, std::string ip, int port);

    igris::trent getsettings(const igris::trent& tr); 

private:
    void beam_thread_func();
};

#endif