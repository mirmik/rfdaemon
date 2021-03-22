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
#include <algorithm>
#include <functors.h>

typedef struct
{
	uint32_t rxOffset;
	uint32_t txOffset;
} CmdBufResult;

template<typename T>
class TcpServer
{
public:

	typedef struct
	{
		uint32_t bitCode;
		Func<T, CmdBufResult, uint8_t*, uint8_t*, uint32_t, uint32_t> cmd;
	} SrvCmd;
	TcpServer(uint16_t port, size_t bufferSize = 65535)
	{
		rxPtr = (uint8_t*)malloc(bufferSize);
		txPtr = (uint8_t*)malloc(bufferSize);
		bufferLength = bufferSize;

		socketDesc = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (socketDesc == -1)
		{
			perror("Socket creation error");
			exit(EXIT_FAILURE);
		}
		memset(&sAddr, 0, sizeof(sAddr));
		sAddr.sin_family = PF_INET;
		sAddr.sin_port = htons(port);
		sAddr.sin_addr.s_addr = htonl(INADDR_ANY);

		if (bind(socketDesc, (sockaddr*)&sAddr, sizeof(sAddr)) == -1)
		{
			perror("Socket bind error");
			close(socketDesc);
			exit(EXIT_FAILURE);
		}

		if (listen(socketDesc, 1) == -1)
		{
			perror("Socket listening error");
			close(socketDesc);
			exit(EXIT_FAILURE);
		}
	}
	~TcpServer()
	{
		free(rxPtr);
		free(txPtr);
	}
	int exec()
	{
		// Main thread loop
		for (;;)
		{
			connDesc = accept(socketDesc, 0, 0);
			if (connDesc < 0)
			{
				perror("Socket accept error");
				close(socketDesc);
				return 1;
			}

			// Main send/receive loop if connection established
			long int result = 0;
			do
			{
				result = recv(socketDesc, rxPtr, bufferLength, 0);
				if (result > 0)
				{
					uint32_t answerOffset = 0, rxDataOffset = 0, cmd = *(uint32_t*)rxPtr;
					for (auto& c : commands)
					{
						if (cmd & c.bitCode)
						{
							CmdBufResult res = c.cmd(rxPtr, txPtr, bufferLength - rxDataOffset, bufferLength - answerOffset);
							answerOffset += res.txOffset;
							rxDataOffset += res.rxOffset;
						}
					}

					result = send(socketDesc, txPtr, std::min<int>(answerOffset, bufferLength), 0);
					if (result < 0)
						printf("Socket send error.");
				}
				else
					printf("Socket receive error.");

				if (terminate)
					break;
			} while (result > 0);

			printf("Socket transfer loop error %d, restart connection.", result);
			shutdown(connDesc, SHUT_RDWR);
			close(connDesc);

			if (terminate)
				return 0;
		}
		return 0;
	}
	void addCmd(uint32_t code, const Func<T, CmdBufResult, uint8_t*, uint8_t*, uint32_t, uint32_t>& cmd)
	{
		SrvCmd c;
		c.bitCode = code;
		c.cmd = cmd;
		commands.push_back(c);
	}
private:
	uint8_t* rxPtr, * txPtr;
	sockaddr_in sAddr;
	int socketDesc = 0;
	int connDesc = 0;
	uint32_t bufferLength = 0;
	bool terminate = false;
	std::vector<SrvCmd> commands;
};
