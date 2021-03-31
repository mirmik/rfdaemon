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
#include <string>
#include <unistd.h>
#include <stdint.h>
#include <algorithm>
#include <pthread.h>

class TcpClient
{
public:
	TcpClient(uint16_t port, size_t bufferSize = 65535)
	{
		rxBufferPtr = (uint8_t*)malloc(bufferSize);
		txBufferPtr = (uint8_t*)malloc(bufferSize);
		bufferLength = bufferSize;

		socketDesc = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (socketDesc == -1)
		{
			perror("Socket creation error.\n");
			exit(EXIT_FAILURE);
		}
		rxQueue.reserve(bufferSize);
		txQueue.reserve(bufferSize);
		pthread_mutex_init(&mtxQueue, NULL);
	}
	~TcpClient()
	{
		rxQueue.clear();
		txQueue.clear();
		free(rxBufferPtr);
		free(txBufferPtr);
	}
	int connectToPort(const std::string& ip, uint16_t port)
	{
		std::string ipaddr = (ip == "localhost") ? "127.0.0.1" : ip;

		memset(&sAddr, 0, sizeof(sAddr));
		sAddr.sin_family = PF_INET;
		sAddr.sin_port = htons(port);

		if (inet_pton(PF_INET, ipaddr.c_str(), &sAddr.sin_addr) <= 0)
		{
			printf("Bad address.\n");
			close(socketDesc);
			return EXIT_FAILURE;
		}

		if (connect(socketDesc, (sockaddr*)&sAddr, sizeof(sAddr)) < 0)
		{
			printf("Bad address.\n");
			close(socketDesc);
			return EXIT_FAILURE;
		}
		connected = true;
		return 0;
	}
	int receiveThread()
	{
		// Main thread loop
		for (;;)
		{
			if (connected)
			{
				ssize_t result = 0;
				do
				{
					result = recv(socketDesc, rxBufferPtr, bufferLength, 0);
					if (result > 0)
					{
						rxQueue.insert(rxQueue.end(), rxBufferPtr, rxBufferPtr + result);
						char lastCh = rxQueue.back();
						if ((lastCh == '\r') || (lastCh == '\n') || (lastCh == 0))
						{
							parseReceivedData(rxQueue);
							rxQueue.clear();
						}
					}
					if (terminate)
						break;
				} while (result > 0);
				connected = false;
				printf("TcpClient receive from server error.\n", result);
				shutdown(connDesc, SHUT_RDWR);
				close(connDesc);
				connDesc = 0;
				if (terminate)
					return 0;
			}
		}
		return 0;
	}
	int sendThread()
	{
		for (;;)
		{
			if (connected)
			{
				// Connection established
				if (!txQueue.empty())
				{
					// Transmission queue not null
					size_t dataOffset = 0;
					ssize_t result = 0;
					pthread_mutex_lock(&mtxQueue);

					// Header and first part has been sent, transmit next parts
					if (txQueuePos < txQueue.size())
					{
						// Multipacket transmission not finished, copy next data part to buffer
						size_t packetSize = std::min(txQueue.size() - txQueuePos, bufferLength);
						memcpy(txBufferPtr + dataOffset, txQueue.data() + txQueuePos, packetSize - dataOffset);

						// Send next data part
						result = send(socketDesc, txBufferPtr, packetSize, 0);
						if (result > 0)
							txQueuePos += result;
						else if (result < 0)
						{
							printf("Client queue send error. Disconnect.\n");
							connected = false;
						}
					}
					else
					{
						// Multipacket transmission finished, clear buffer
						txQueue.clear();
						txQueuePos = 0;
					}
					pthread_mutex_unlock(&mtxQueue);
					if (result < 0)
						printf("Socket send error.\n");
				}
			}
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
	virtual void parseReceivedData(const std::vector<uint8_t>& data) = 0;
	void sendCmd(const std::string& cmd, bool wait = false, int timeoutMs = 0)
	{
		pthread_mutex_lock(&mtxQueue);
		txQueue.insert(txQueue.begin(), cmd.c_str(), cmd.c_str() + cmd.length() + 1);
		pthread_mutex_unlock(&mtxQueue);
		int cnt = timeoutMs;
		while (!txQueue.empty() && cnt--)
			usleep(1000);
	}
private:
	bool connected = false;
	size_t bufferLength = 0;
	size_t txQueuePos = 0;
	uint8_t* rxBufferPtr, * txBufferPtr;
	std::vector<uint8_t> rxQueue, txQueue;
	sockaddr_in sAddr;
	int socketDesc = 0;
	int connDesc = 0;
	bool terminate = false;
	pthread_mutex_t mtxQueue;
};
