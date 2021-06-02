#include "TcpServer.h"
#include <cstring>
#include <unistd.h>
#include <signal.h>
#include "igris/util/crc.h"
#include "arpa/inet.h"

using namespace std;

TcpServer::TcpServer(uint16_t port, size_t bufferSize)
{
    bufferLength = bufferSize;
    rxQueue.reserve(bufferSize * 2);
    txQueue.reserve(bufferSize * 2);

    // Prevent crash due to broken socket pipe
    signal(SIGPIPE, SIG_IGN);
    socketDesc = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socketDesc == -1)
    {
        perror("Socket creation error.\n");
        exit(EXIT_FAILURE);
    }
    int reuse = 1;
    if (setsockopt(socketDesc, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0)
        printf("setsockopt(SO_REUSEADDR) failed");

    memset(&sAddr, 0, sizeof(sAddr));
    sAddr.sin_family = PF_INET;
    sAddr.sin_port = htons(port);
    sAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(socketDesc, (sockaddr*)&sAddr, sizeof(sAddr)) == -1)
    {
        perror("Socket bind error.\n");
        close(socketDesc);
        exit(EXIT_FAILURE);
    }

    if (listen(socketDesc, 1) == -1)
    {
        perror("Socket listening error.\n");
        close(socketDesc);
        exit(EXIT_FAILURE);
    }
}

TcpServer::~TcpServer()
{
    if (socketDesc != -1)
        close(socketDesc);
    rxQueue.clear();
    txQueue.clear();
    free(rxBufferPtr);
    free(txBufferPtr);
}

int TcpServer::receiveThread()
{
    for (;;)
    {
        if (!connectionAccepted)
        {
            usleep(1000);
            connDesc = accept(socketDesc, 0, 0);
            if (connDesc < 0)
            {
                perror("Socket accept error.\n");
                close(socketDesc);
                return 1;
            }
            else
                connectionAccepted = true;
        }
        else
        {
            ssize_t result = 0;
            do
            {
                result = recv(connDesc, rxBufferPtr + rxBufferHeaderCollectCnt, bufferLength, 0);

                if (result > 0)
                {
                    uint8_t* dataStartPtr = rxBufferPtr;

                    if (!rxQueueActive)
                    {
                        if (rxBufferHeaderCollectCnt < sizeof(PacketHeader))
                            rxBufferHeaderCollectCnt += result;

                        if (rxBufferHeaderCollectCnt >= sizeof(PacketHeader))
                        {
                            result = rxBufferHeaderCollectCnt;
                            rxBufferHeaderCollectCnt = 0; // Clear header fragment size counter

                            // Received data contains header at start
                            PacketHeader* h = (PacketHeader*)&rxBufferPtr;
                            if (h->preamble == HeaderPreamble)
                            {
                                rxQueueActive = true;
                                rxQueue.clear();
                                currentHeader = *h;
                                dataStartPtr = rxBufferPtr + sizeof(PacketHeader);
                                result -= sizeof(PacketHeader);
                                if (result >= h->size)
                                    rxQueue.insert(rxQueue.end(), dataStartPtr, dataStartPtr + result);
                            }
                        }
                    }

                    if (rxQueueActive && (result > 0))
                    {
                        // Received data is not header, it is body part
                        if (rxQueue.size() < currentHeader.size)
                            rxQueue.insert(rxQueue.end(), dataStartPtr, dataStartPtr + result);
                        else
                        {
                            rxQueueActive = false;
                            if (crc32(rxQueue.data(), currentHeader.size, 0) == currentHeader.crc32)
                            {
                                while (txQueueActive);
                                mQueue.lock();
                                txQueue = parseReceivedData(rxQueue);
                                if (txQueue.empty())
                                    lastQueryResult = QueryResult::AppRejected;
                                else
                                    lastQueryResult = QueryResult::AllOk;
                                mQueue.unlock();
                            }
                            else
                                lastQueryResult = QueryResult::CRCError;
                            rxQueue.clear();
                        }
                    }
                }
                if (terminate)
                    break;
            } while (result > 0);
            rxQueueActive = false;
            connectionAccepted = false;
            if (result != 0)
                printf("Socket receive error %d, restart connection.\n", (int)result);
            else
                printf("Client disconnected.\n");
            shutdown(connDesc, SHUT_RDWR);
            if (terminate)
                break;
        }
    }
    return 0;
}

int TcpServer::sendThread()
{
    for (;;)
    {
        if (connectionAccepted)
        {
            // Connection established
            if (!txQueue.empty())
            {
                // Transmission queue not null
                int headerOffset = 0;
                ssize_t result = 0;
                mQueue.lock();

                // Header and first data part not sent yet
                if (!txQueueActive)
                {
                    txQueueActive = true;
                    PacketHeader* h = (PacketHeader*)txBufferPtr;
                    h->preamble = HeaderPreamble;
                    h->size = txQueue.size();
                    h->crc32 = crc32(txQueue.data(), h->size, 0);
                    txQueuePos = 0;
                    headerOffset = sizeof(PacketHeader);
                }

                // Header and first part has been sent, transmit next parts
                if (txQueueActive)
                {
                    if (txQueuePos < txQueue.size())
                    {
                        // Multipacket transmission not finished, copy next data part to buffer
                        size_t packetSize = std::min(txQueue.size() - txQueuePos + headerOffset, bufferLength);
                        memcpy(txBufferPtr + headerOffset, txQueue.data() + txQueuePos,
                            packetSize - headerOffset);
                        
                        // Send next data part
                        result = send(connDesc, txBufferPtr, packetSize, 0);
                        if (result > 0)
                            txQueuePos += result - headerOffset;
                        else if (result < 0)
                        {
                            // In case of error at header transfer, reset header include flag
                            if (headerOffset)
                                txQueueActive = false;
                            printf("Server packet header send error.\n");
                        }
                    }
                    else
                    {
                        // Multipacket transmission finished, clear buffer
                        txQueue.clear();
                        txQueuePos = 0;
                        txQueueActive = false;
                    }
                }
                mQueue.unlock();
                if (result < 0)
                    printf("Socket send error.\n");
            }
        }
        else
            usleep(1000);
        if (terminate)
            break;
    }
    return 0;
}

size_t TcpServer::getRxQueueSize() const
{
    return rxQueue.size();
}

size_t TcpServer::getTxQueueSize() const
{
    return txQueue.size();
}

size_t TcpServer::getBufferSize() const
{
    return bufferLength;
}

bool TcpServer::clientConnected()
{
    return connectionAccepted;
}

string TcpServer::getClientInfo()
{
    char info[128];
    char* client_ip = inet_ntoa(sAddr.sin_addr);
    int client_port = ntohs(sAddr.sin_port);
    if (client_ip != nullptr && connectionAccepted)
        snprintf(info, sizeof(info), "IP:%s Port:%d", client_ip, client_port);
    else
        snprintf(info, sizeof(info), "Unable to get client data");
    return string(info);
}
