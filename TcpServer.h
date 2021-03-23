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
	static constexpr uint32_t answerMask = 1 << 31;
	static constexpr uint32_t ackMask = 1 << 30;
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
	int thread()
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
					uint32_t cmd = *(uint32_t*)rxPtr;
					uint32_t answerOffset = sizeof(cmd), rxDataOffset = sizeof(cmd);
					for (auto& c : commands)
					{
						if (cmd & c.bitCode)
						{
							CmdBufResult res = c.cmd(rxPtr + rxDataOffset,
													 txPtr + answerOffset,
													 bufferLength - rxDataOffset,
													 bufferLength - answerOffset - streamingDataLength);
							answerOffset += res.txOffset;
							rxDataOffset += res.rxOffset;
						}
					}
					*(uint32_t*)txPtr = (answerOffset > sizeof(cmd)) ? (cmd | answerMask) : 0;

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
	uint32_t getBufferLength()
	{
		return bufferLength;
	}
	void streamingEnable(uint32_t streamingLen)
	{
		streamingEnabled = true;
		streamingDataLength = streamingLen;
	}
	void streamingDisable()
	{
		streamingEnabled = false;
		streamingDataLength = 0;
	}
private:
	uint32_t bufferLength = 0;
	uint8_t* rxPtr, * txPtr;
	sockaddr_in sAddr;
	int socketDesc = 0;
	int connDesc = 0;
	bool terminate = false;
	std::vector<SrvCmd> commands;
	bool streamingEnabled = false;
	uint32_t streamingDataLength = 0;
};
