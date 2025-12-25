#pragma once

#include <atomic>
#include <mutex>
#include <nos/inet/tcp_client.h>
#include <nos/inet/tcp_server.h>
#include <stdint.h>
#include <vector>
#include <thread>
#include <igris/container/dlist.h>

struct PacketHeader
{
    uint32_t preamble;
    uint32_t crc32;
    uint32_t size;

    bool operator==(const PacketHeader &other) const
    {
        return preamble == other.preamble && crc32 == other.crc32 && size == other.size;
    }

    nos::expected<size_t, nos::output_error> print_to(nos::ostream &o) const
    {
        return nos::fprint_to(o, "preamble: {}, crc32: {}, size: {}", preamble, crc32, size);
    }
};

enum class QueryResult
{
    AllOk = 0,
    CRCError,
    AppRejected
};

class TcpServer;

struct ClientStruct 
{
    igris::dlist_node lnk;
    nos::inet::tcp_client client;
    std::thread receive_thread;
    TcpServer * tcp_server;
    PacketHeader currentHeader;

    ClientStruct(nos::inet::tcp_client client, TcpServer* tcp_server);
    void run();
    void start_receive_thread();
    bool send(std::vector<uint8_t> data);
};

class TcpServer
{
public:
    TcpServer(uint16_t port);
    ~TcpServer();
    int receiveThread();
    bool clientConnected();
    void stop();
    virtual std::vector<uint8_t>
    parseReceivedData(const std::vector<uint8_t> &data) = 0;

    std::vector<ClientStruct*> marked_for_delete;

    void mark_as_deleted(ClientStruct *client)
    {
        std::lock_guard<std::mutex> lock(mQueue);
        marked_for_delete.push_back(client);
        client->lnk.unlink();
    }

    void delete_marked()
    {
        std::vector<ClientStruct*> to_delete;
        {
            std::lock_guard<std::mutex> lock(mQueue);
            to_delete = std::move(marked_for_delete);
            marked_for_delete.clear();
        }
        // Join threads outside of lock to avoid deadlock
        for (auto client : to_delete)
        {
            delete_client(client);
        }
    }

    void delete_client(ClientStruct *client)
    {   
        if (client->receive_thread.joinable())
        {
            client->receive_thread.join();
        }
        delete client;
    }

public:
    void setupConnection();
    static constexpr uint32_t HeaderPreamble = 0x69EA23BE;
    //bool connectionCreated = false;
    //bool connectionAccepted = false;
    // Byte counter for receiving packet header in multistep mode
    // when recv() returns less bytes than size of header
    //size_t rxBufferHeaderCollectCnt = 0;
    // uint8_t rxBufferPtr[65535 + sizeof(PacketHeader)],
    //     txBufferPtr[65535 + sizeof(PacketHeader)];
    // size_t bufferLength = 0;
    //size_t txQueuePos = 0;
    //bool rxQueueActive = false;
    //bool txQueueActive = false;
    //std::vector<uint8_t> rxQueue, txQueue;
    //PacketHeader currentHeader;
    //QueryResult lastQueryResult = QueryResult::AllOk;
    uint16_t usedPort = 0;
    nos::inet::tcp_server socket;
    std::atomic<bool> stopped{false};

    igris::dlist<ClientStruct, &ClientStruct::lnk> clients;

public:
    std::mutex mQueue;
};
