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

class TcpClient
{
public:
	TcpClient(uint16_t port, int bufferSize = 65535)
	{
		rxPtr = (uint8_t*)malloc(bufferSize);
		txPtr = (uint8_t*)malloc(bufferSize);
		bufferLength = bufferSize;

		socketDesc = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (socketDesc == -1)
		{
			perror("Socket creation error.\n");
			exit(EXIT_FAILURE);
		}
	}
	~TcpClient()
	{
		free(rxPtr);
		free(txPtr);
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
	}
	int socketSend(const char* data, uint32_t size)
	{
		int transferLen = std::min(size, bufferLength);
		//memcpy(txPtr, data, transferLen);
		if (send(socketDesc, data, transferLen, 0) < 0)
		{
			printf("TCP client send failed.\n");
			close(socketDesc);
			return EXIT_FAILURE;
		}
		else
			return 0;
	}
	int socketReceive(char* data, uint32_t size)
	{
		int transferLen = std::min(size, bufferLength);
		if (recv(socketDesc, data, transferLen, 0) < 0)
		{
			printf("TCP client receive failed.\n");
			close(socketDesc);
			return EXIT_FAILURE;
		}
		else
		{
			//memcpy(data, rxPtr, transferLen);
			return 0;
		}
	}
	int thread()
	{
		for (;;)
		{
			if (txSize)
				socketSend((const char*)txPtr, txSize);
			if (rxSize)
				socketReceive((char*)rxPtr, rxSize);
		}
		return 0;
	}
	uint32_t getBufferLength()
	{
		return bufferLength;
	}
private:
	uint32_t bufferLength = 0;
	uint8_t* rxPtr, * txPtr;
	uint32_t txSize = 0, rxSize = 0;
	sockaddr_in sAddr;
	int socketDesc = 0;
	int connDesc = 0;
	bool terminate = false;
};
