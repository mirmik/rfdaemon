#include "TcpServer.h"
#include "arpa/inet.h"
#include "crc32_ccitt.h"
#include <cstring>
#include <errno.h>
#include <modes.h>
#include <shutdown.h>
#include <signal.h>
#include <sys/select.h>
#include <thread>
#include <unistd.h>
#include <igris/util/dstring.h>

TcpServer::TcpServer(uint16_t port)
{
    //bufferLength = bufferSize;
    //rxQueue.reserve(bufferSize * 2);
    //txQueue.reserve(bufferSize * 2);

    // Prevent crash due to broken socket pipe
    signal(SIGPIPE, SIG_IGN);
    usedPort = port;
    //setupConnection();
}

TcpServer::~TcpServer()
{
    stop();
}

void TcpServer::stop()
{
    nos::fprintln("[TcpServer] stop() called, thread ID: {}", std::this_thread::get_id());
    stopped.store(true);
    nos::println("[TcpServer] Closing socket...");
    socket.close();
    nos::println("[TcpServer] Socket closed");

    // Clean up all clients
    nos::println("[TcpServer] Acquiring mutex for client cleanup...");
    std::lock_guard<std::mutex> lock(mQueue);
    nos::fprintln("[TcpServer] Cleaning up {} clients...", clients.size());
    while (!clients.empty())
    {
        ClientStruct* c = &clients.front();
        nos::println("[TcpServer] Cleaning up client...");
        c->lnk.unlink();
        c->client.close();
        if (c->receive_thread.joinable())
        {
            nos::println("[TcpServer] Joining client receive thread...");
            c->receive_thread.join();
            nos::println("[TcpServer] Client receive thread joined");
        }
        delete c;
    }

    nos::fprintln("[TcpServer] Cleaning up {} marked_for_delete clients...", marked_for_delete.size());
    for (auto* c : marked_for_delete)
    {
        if (c->receive_thread.joinable())
        {
            nos::println("[TcpServer] Joining marked client receive thread...");
            c->receive_thread.join();
            nos::println("[TcpServer] Marked client receive thread joined");
        }
        delete c;
    }
    marked_for_delete.clear();
    nos::println("[TcpServer] stop() done");
}

    ClientStruct::ClientStruct(nos::inet::tcp_client client, TcpServer* tcp_server) : client(client) 
    {
        this->tcp_server = tcp_server;
    }

    nos::expected<PacketHeader, nos::output_error> ClientStruct::read_header() {
        PacketHeader header;
        auto ret = client.recv((char*)&header, sizeof(PacketHeader), MSG_WAITALL);
        if (ret.is_error()) {
            return nos::output_error();
        }
        if (*ret != sizeof(PacketHeader)) {
            return nos::output_error();
        }
        return header;
    }

    void ClientStruct::run()
    {
        nos::fprintln("[ClientStruct] run() started, thread ID: {}", std::this_thread::get_id());
        std::vector<uint8_t> data;
        while(1)
        {
            nos::expected<PacketHeader, nos::output_error> errheader = read_header();
            if (errheader.is_error()) 
            {
                nos::println("Socket receive error.");
                goto finish;
            }
            PacketHeader header = *errheader;
            size_t size = header.size;

            if (size > data.size())
                data.resize(size);

            auto ret = client.recv((char*)data.data(), size, MSG_WAITALL);
            if (ret.is_error()) 
            {
                nos::println("Socket receive error.");
                goto finish;
            }
            if (*ret != size) 
            {
                nos::println("Socket receive error.");
                goto finish;
            }            
            size_t datacrc = crc32_ccitt(data.data(), size, 0);
            if (datacrc == header.crc32)
            {
                data.resize(size+1024);
                auto sdata = tcp_server->parseReceivedData(data);
                if (!send(sdata))
                {
                    nos::println("Socket send error (client disconnected).");
                    goto finish;
                }
            }
            else 
            {
                nos::println("CRC error");
                goto finish;
            }
        }

        finish:
            nos::println("[ClientStruct] run() finishing, closing client...");
            client.close();
            tcp_server->mark_as_deleted(this);
            nos::println("[ClientStruct] run() done");
    }

    void ClientStruct::start_receive_thread()
    {
        nos::fprintln("[ClientStruct] Starting receive thread from thread {}", std::this_thread::get_id());
        receive_thread = std::thread([this](){ run(); });
    }

bool ClientStruct::send(std::vector<uint8_t> data)
{
        if (data.empty())
        {
            return true;
        }

        try {
            PacketHeader h;
            h.preamble = tcp_server->HeaderPreamble;
            h.size = (uint32_t)data.size();
            h.crc32 = crc32_ccitt(data.data(), h.size, 0);
            client.write((char*)&h, sizeof(PacketHeader));
            client.write((char*)data.data(), data.size());
            return true;
        } catch (const nos::inet::tcp_write_error&) {
            return false;
        }
}

int TcpServer::receiveThread()
{
    nos::fprintln("[TcpServer] receiveThread started, thread ID: {}", std::this_thread::get_id());
    socket.init();
    socket.reusing(true);

    if (socket.bind("0.0.0.0", usedPort) == -1)
    {
        perror("Socket bind error.\n");
        nos::println("Socket port:" + std::to_string(usedPort));
        socket.close();
        nos::println("Exit because of bind error");
        exit(EXIT_FAILURE);
    }

    if (socket.listen(1) == -1)
    {
        perror("Socket listening error.\n");
        socket.close();
        exit(EXIT_FAILURE);
    }

    while (!stopped.load() && !is_shutdown_requested())
    {
        delete_marked();

        // Use select with timeout to make accept() interruptible
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(socket.fd(), &readfds);

        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 100000; // 100ms timeout

        int ret = select(socket.fd() + 1, &readfds, nullptr, nullptr, &tv);
        if (ret < 0)
        {
            if (errno == EINTR)
                continue;
            break;
        }
        if (ret == 0)
            continue; // timeout, check shutdown flag

        if (!FD_ISSET(socket.fd(), &readfds))
            continue;

        auto client = socket.accept();
        if (client.good())
        {
            ClientStruct * c = new ClientStruct(client, this);
            clients.move_back(*c);
            c->start_receive_thread();
        }
        else
        {
            if (stopped.load() || is_shutdown_requested())
                break;
            return -1;
        }
    }

    nos::println("TcpServer receiveThread stopped");
    socket.close();
    return 0;
}




// int TcpServer::receiveThread()
// {
//     while (true)
//     {
//         if (!connectionAccepted)
//         {
//             usleep(10000);
//             if (!connectionCreated)
//                 setupConnection();
//             connection = socket.accept();
//             if (!connection.good())
//             {
//                 perror("Socket accept error.\n");
//                 socket.close();
//                 break;
//             }

//             connectionAccepted = true;
//         }
//         else
//         {
//             ssize_t result = 0;
//             do
//             {
//                 char *buffer = (char *)rxBufferPtr + rxBufferHeaderCollectCnt;
//                 auto eresult = connection.recv(buffer, bufferLength, 0);
//                 if (eresult.is_ok())
//                     result = *eresult;
//                 else
//                     result = -1;

//                 if (result > 0)
//                 {
//                     uint8_t *dataStartPtr = rxBufferPtr;

//                     if (!rxQueueActive)
//                     {
//                         if (rxBufferHeaderCollectCnt < sizeof(PacketHeader))
//                             rxBufferHeaderCollectCnt += result;

//                         if (rxBufferHeaderCollectCnt >= sizeof(PacketHeader))
//                         {
//                             result = rxBufferHeaderCollectCnt;
//                             rxBufferHeaderCollectCnt =
//                                 0; // Clear header fragment size counter

//                             // Received data contains header at start
//                             PacketHeader *h = (PacketHeader *)&rxBufferPtr;
//                             if (h->preamble == HeaderPreamble)
//                             {
//                                 rxQueueActive = true;
//                                 rxQueue.clear();
//                                 currentHeader = *h;
//                                 dataStartPtr =
//                                     rxBufferPtr + sizeof(PacketHeader);
//                                 result -= sizeof(PacketHeader);
//                                 if ((size_t)result >= h->size)
//                                     rxQueue.insert(rxQueue.end(), dataStartPtr,
//                                                    dataStartPtr + result);
//                             }
//                         }
//                     }

//                     if (rxQueueActive && (result > 0))
//                     {
//                         // Received data is not header, it is body part
//                         if (rxQueue.size() < currentHeader.size)
//                             rxQueue.insert(rxQueue.end(), dataStartPtr,
//                                            dataStartPtr + result);
//                         else
//                         {
//                             rxQueueActive = false;
//                             if (crc32_ccitt(rxQueue.data(), currentHeader.size,
//                                             0) == currentHeader.crc32)
//                             {
//                                 while (txQueueActive)
//                                     ;
//                                 mQueue.lock();
//                                 txQueue = parseReceivedData(rxQueue);
//                                 if (txQueue.empty())
//                                     lastQueryResult = QueryResult::AppRejected;
//                                 else
//                                     lastQueryResult = QueryResult::AllOk;
//                                 mQueue.unlock();
//                             }
//                             else
//                                 lastQueryResult = QueryResult::CRCError;
//                             rxQueue.clear();
//                         }
//                     }
//                 }
//                 if (terminateRxThread)
//                     break;
//             } while (result > 0);
//             rxQueueActive = false;
//             connectionAccepted = false;
//             if (result != 0)
//                 printf("Socket receive error %d, restart connection.\n",
//                        (int)result);
//             else
//                 printf("Client disconnected.\n");
//             mConn.lock();
//             connection.close();
//             mConn.unlock();
//             socket.close();
//             connectionCreated = false;
//         }
//         if (terminateRxThread)
//             break;
//     }
//     terminateRxThread = false;
//     return 0;
// }

// int TcpServer::sendThread()
// {
//     for (;;)
//     {
//         if (connectionAccepted)
//         {
//             // Connection established
//             if (!txQueue.empty())
//             {
//                 // Transmission queue not null
//                 int headerOffset = 0;
//                 ssize_t result = 0;
//                 mQueue.lock();

//                 // Header and first data part not sent yet
//                 if (!txQueueActive)
//                 {
//                     txQueueActive = true;
//                     PacketHeader *h = (PacketHeader *)txBufferPtr;
//                     h->preamble = HeaderPreamble;
//                     h->size = (uint32_t)txQueue.size();
//                     h->crc32 = crc32_ccitt(txQueue.data(), h->size, 0);
//                     txQueuePos = 0;
//                     headerOffset = sizeof(PacketHeader);
//                 }

//                 // Header and first part has been sent, transmit next parts
//                 if (txQueueActive)
//                 {
//                     if (txQueuePos < txQueue.size())
//                     {
//                         // Multipacket transmission not finished, copy next data
//                         // part to buffer
//                         size_t packetSize =
//                             std::min(txQueue.size() - txQueuePos + headerOffset,
//                                      bufferLength);
//                         memcpy(txBufferPtr + headerOffset,
//                                txQueue.data() + txQueuePos,
//                                packetSize - headerOffset);

//                         // Send next data part
//                         mConn.lock();

//                         if (VERBOSE)
//                         {
//                             nos::println("Sending packet: ");
//                             nos::print_dump(txBufferPtr, packetSize);
//                         }

//                         auto eresult =
//                             connection.write(txBufferPtr, packetSize);
//                         if (eresult.is_ok())
//                             result = *eresult;
//                         else
//                             result = -1;

//                         mConn.unlock();
//                         if (result > 0)
//                             txQueuePos += result - headerOffset;
//                         else if (result < 0)
//                         {
//                             // In case of error at header transfer, reset header
//                             // include flag
//                             if (headerOffset)
//                                 txQueueActive = false;
//                             printf("Server packet header send error.\n");
//                         }
//                     }
//                     else
//                     {
//                         // Multipacket transmission finished, clear buffer
//                         txQueue.clear();
//                         txQueuePos = 0;
//                         txQueueActive = false;
//                     }
//                 }
//                 mQueue.unlock();
//                 if (result < 0)
//                     printf("Socket send error.\n");
//             }
//             else
//                 usleep(1000);
//         }
//         else
//             usleep(10000);
//         if (terminateTxThread)
//             break;
//     }
//     terminateTxThread = false;
//     return 0;
// }

// size_t TcpServer::getRxQueueSize() const
// {
//     return rxQueue.size();
// }

// size_t TcpServer::getTxQueueSize() const
// {
//     return txQueue.size();
// }

// size_t TcpServer::getBufferSize() const
// {
//     return bufferLength;
// }

// bool TcpServer::clientConnected()
// {
//     return connectionAccepted;
// }

// void TcpServer::setupConnection()
// {

//     socket.init();
//     socket.reusing(true);

//     if (socket.bind("0.0.0.0", usedPort) == -1)
//     {
//         perror("Socket bind error.\n");
//         socket.close();
//         exit(EXIT_FAILURE);
//     }

//     if (socket.listen(1) == -1)
//     {
//         perror("Socket listening error.\n");
//         socket.close();
//         exit(EXIT_FAILURE);
//     }
//     //connectionCreated = true;
// }
