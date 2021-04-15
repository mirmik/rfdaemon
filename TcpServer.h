#pragma once

#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <signal.h>
#include <algorithm>
#include <functors.h>
#include "igris/util/crc.h"

typedef struct
{
	uint32_t preamble;
	uint32_t crc32;
	uint32_t size;
} PacketHeader;

enum class QueryResult
{
	AllOk = 0,
	CRCError,
	AppRejected
};

class TcpServer
{
public:
	TcpServer(uint16_t port, size_t bufferSize = 65535)
	{
		//rxBufferPtr = (uint8_t*)malloc(bufferSize + sizeof(PacketHeader));
		//txBufferPtr = (uint8_t*)malloc(bufferSize + sizeof(PacketHeader));
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
		pthread_mutex_init(&mtxQueue, NULL);
	}
	~TcpServer()
	{
		rxQueue.clear();
		txQueue.clear();
		free(rxBufferPtr);
		free(txBufferPtr);
	}
	int receiveThread()
	{
		// Main thread loop
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
									pthread_mutex_lock(&mtxQueue);
									txQueue = parseReceivedData(rxQueue);
									if (txQueue.empty())
										lastQueryResult = QueryResult::AppRejected;
									else
										lastQueryResult = QueryResult::AllOk;
									pthread_mutex_unlock(&mtxQueue);
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
				printf("Socket receive error %d, restart connection.\n", result);
				shutdown(connDesc, SHUT_RDWR);
				if (terminate)
					break;
			}
		}
		return 0;
	}
	int sendThread()
	{
		for (;;)
		{
			if (connectionAccepted)
			{
				// Connection established
				if (!txQueue.empty())
				{
					// Transmission queue not null
					size_t dataOffset = 0;
					ssize_t result = 0;
					pthread_mutex_lock(&mtxQueue);

					// Header and first data part not sent yet
					if (!txQueueActive)
					{
						txQueueActive = true;
						PacketHeader* h = (PacketHeader*)txBufferPtr;
						h->preamble = HeaderPreamble;
						h->size = txQueue.size();
						h->crc32 = crc32(txQueue.data(), h->size, 0);
						txQueuePos = 0;
						dataOffset = sizeof(PacketHeader);
					}

					// Header and first part has been sent, transmit next parts
					if (txQueueActive)
					{
						if (txQueuePos < txQueue.size())
						{
							// Multipacket transmission not finished, copy next data part to buffer
							size_t packetSize = std::min(txQueue.size() - txQueuePos + dataOffset, bufferLength);
							memcpy(txBufferPtr + dataOffset, txQueue.data() + txQueuePos, packetSize - dataOffset);
							
							// Send next data part
							result = send(connDesc, txBufferPtr, packetSize, 0);
							if (result > 0)
								txQueuePos += result;
							else if (result < 0)
							{
								// In case of error at header transfer, reset header include flag
								if (dataOffset)
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
					pthread_mutex_unlock(&mtxQueue);
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
	size_t getRxQueueSize() const
	{
		return rxQueue.size();
	}
	size_t getTxQueueSize() const
	{
		return txQueue.size();
	}
	size_t getBufferSize() const
	{
		return bufferLength;
	}
	bool clientConnected()
	{
		return connectionAccepted;
	}
	virtual std::vector<uint8_t> parseReceivedData(const std::vector<uint8_t>& data) = 0;
private:
	static constexpr uint32_t HeaderPreamble = 0x69EA23BE;
	bool connectionAccepted = false;
	size_t bufferLength = 0;
	size_t txQueuePos = 0;
	size_t rxBufferHeaderCollectCnt = 0; // Byte counter for receiving packet header in multistep mode when recv() returns less bytes than size of header
	uint8_t rxBufferPtr[65535 + sizeof(PacketHeader)], txBufferPtr[65535 + sizeof(PacketHeader)];
	bool rxQueueActive = false;
	bool txQueueActive = false;
	std::vector<uint8_t> rxQueue, txQueue;
	PacketHeader currentHeader;
	QueryResult lastQueryResult = QueryResult::AllOk;
	sockaddr_in sAddr;
	int socketDesc = 0;
	int connDesc = 0;
	bool terminate = false;
	pthread_mutex_t mtxQueue;
};
