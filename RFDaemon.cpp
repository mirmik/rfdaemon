#include "RFDaemon.h"
#include <unistd.h>
#include <algorithm>
#include "igris/util/crc.h"

RFDaemon::RFDaemon(uint16_t port, uint16_t bufferLen)
{
    bufferLength = bufferLen;
    rxBuf.resize(bufferLength);
    txBuf.resize(bufferLength);
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

int RFDaemon::exec()
{
    // Main thread loop
	for (;;)
	{
		connDesc = accept(socketDesc, 0, 0);
		if (connDesc < 0)
		{
			perror("Socket accept error");
			close(socketDesc);
			exit(EXIT_FAILURE);
		}

        // Main send/receive loop if connection established
		long int result = 0, txSize = 0;
		do
		{
            result = recv(socketDesc, rxBuf.data(), sizeof(rxBuf), 0);
            if (result > 0)
            {
                txSize = parseInput(rxBuf.data(), txBuf.data(), result);
                result = send(socketDesc, txBuf.data(), std::min<int>(txSize, sizeof(txBuf)), 0);
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
            break;
	}
	return 0;
}

int RFDaemon::parseInput(uint8_t* input, uint8_t* output, uint32_t inputSize)
{
    static uint32_t multipacketWriteDataPos = 0;
    static uint32_t multipacketReadDataPos = 0;
    static uint32_t multipacketWriteSize = 0;
    static uint32_t multipacketReadSize = 0;
    static uint32_t multipacketCRC32 = 0;
    static bool multipacketReadEnabled = false;
    static bool multipacketWriteEnabled = false;
    static std::vector<uint8_t>* multipacketWritePtr = nullptr;

    uint16_t rxCnt = 4;
    uint16_t answerSize = 4;
    uint32_t cmdWord = *((uint32_t*)input);
    unsigned int answerCmd = 0;
    if (cmdWord & Cmd::IS_ANSWER)
        return 0;

    if (cmdWord & Cmd::SERVICES_START)
    {

    }
    if (cmdWord & Cmd::SERVICES_STOP)
    {

    }
    if (cmdWord & Cmd::SERVICES_RESTART)
    {

    }
    if (cmdWord & Cmd::GET_DEVICES)
    {
        unsigned int answerSizeOld = answerSize;
        const char dev1[] = "Device 1";
        const char dev2[] = "Device 2";
        const char dev3[] = "Device 3";
        const char dev4[] = "Device 4";

        output[answerSize++] = 4;
        memcpy(output + answerSize, dev1, sizeof(dev1));
        answerSize += sizeof(dev1);
        memcpy(output + answerSize, dev2, sizeof(dev2));
        answerSize += sizeof(dev2);
        memcpy(output + answerSize, dev3, sizeof(dev3));
        answerSize += sizeof(dev3);
        memcpy(output + answerSize, dev4, sizeof(dev4));
        answerSize += sizeof(dev4);
        if (answerSizeOld != answerSize)
            answerCmd |= Cmd::GET_DEVICES;
    }
    if (cmdWord & Cmd::GET_LOGS)
    {

    }
    if (cmdWord & Cmd::SET_STREAMING)
    {
        streamingDevNum = input[rxCnt++];
        streamingEnabled = input[rxCnt++];
        answerCmd |= Cmd::SET_STREAMING;
    }
    if (cmdWord & Cmd::AXES_LIMITS_SET)
    {
        uint32_t recordsNum = input[rxCnt++];

        if (recordsNum)
        {
            for (uint32_t i = 0; (i < recordsNum) && (i < limitsList.size()); i++)
            {
                limitsList[i].min = *(double*)(input + rxCnt);
                rxCnt += 8;
                limitsList[i].max = *(double*)(input + rxCnt);
                rxCnt += 8;
            }
            answerCmd |= Cmd::AXES_LIMITS_SET;
        }
    }
    if (cmdWord & Cmd::AXES_LIMITS_GET)
    {
        int recordsNum = limitsList.size();

        if (recordsNum)
        {
            output[answerSize++] = recordsNum;
            for (int i = 0; i < recordsNum; i++)
            {
                *(double*)(output + answerSize) = limitsList[i].min;
                answerSize += 8;
                *(double*)(output + answerSize) = limitsList[i].max;
                answerSize += 8;
            }
            answerCmd |= Cmd::AXES_LIMITS_GET;
        }
    }
    if (cmdWord & Cmd::ZERO_ALL_AXES)
    {

    }
    if (cmdWord & Cmd::HOME_ALL_AXES)
    {

    }
    if (cmdWord & Cmd::STARTUP_SCRIPT_GET)
    {

    }
    if (cmdWord & Cmd::STARTUP_SCRIPT_SET)
    {

    }
    if (cmdWord & Cmd::UPDATE_IMG)
    {
        multipacketWritePtr = &bigFileData;
    }
    if (cmdWord & Cmd::UPDATE_FIRMWARE)
    {

    }
    if (cmdWord & Cmd::PARAMS_READ)
    {
        // Connected device which params need to send
        unsigned char devNum = input[rxCnt++];
        unsigned short paramCount = input[rxCnt] | (input[rxCnt + 1] << 8); // if paramCount == 0xFFFF then we need to send all available parameters
        unsigned int answerSizeOld = answerSize;
        rxCnt += 2;

        if (devNum < parameters.size())
        {
            // request all parameters
            if (paramCount == 0xFFFF)
                answerSize += packParamData(&output[answerSize], devNum, bufferLength - answerSize);
            else
            {
                // request a list of specific parameters
                for (int i = 0; i < paramCount; i++)
                {
                    uint16_t paramId = input[rxCnt] | (input[rxCnt + 1] << 8);
                    rxCnt += 2;
                    answerSize += packParamData(&output[answerSize], devNum, bufferLength - answerSize, paramId);
                }
            }
        }
        if (answerSizeOld != answerSize)
            answerCmd |= Cmd::PARAMS_READ;
    }
    if (cmdWord & Cmd::PARAMS_WRITE)
    {

    }
    if (cmdWord & Cmd::AXIS_SETZERO)
    {

    }
    if (cmdWord & Cmd::AXIS_HOME)
    {

    }
    if (cmdWord & Cmd::AXIS_SETPOS)
    {

    }
    if (cmdWord & Cmd::AXIS_STOP)
    {

    }
    if (cmdWord & Cmd::AXIS_JOG)
    {

    }
    if (cmdWord & Cmd::STREAMING_DATA)
    {
    }
    if (cmdWord & Cmd::BEGIN_MULTIPACKET_READ)
    {
        multipacketReadDataPos = 0;
        multipacketReadSize = fileTransmitSize;
        *((unsigned int*)(output + answerSize)) = multipacketReadSize;
        answerSize += 4;
        *((unsigned int*)(output + answerSize)) = crc32(fileTransmitPtr, fileTransmitSize, true);
        answerSize += 4;
        answerCmd |= Cmd::BEGIN_MULTIPACKET_READ;
        multipacketReadEnabled = true;
    }
    if (cmdWord & Cmd::MULTIPACKET_DATA_READ)
    {
        if (multipacketReadEnabled)
        {
            uint16_t bufFreeSpace = sizeof(output) - answerSize - 2;
            if (streamingEnabled)
                bufFreeSpace -= 5;
            uint16_t partSize = std::min<uint32_t>(bufFreeSpace, fileTransmitSize - multipacketReadDataPos);
            *((uint16_t*)(output + answerSize)) = partSize;
            answerSize += 2;
            memcpy(output + answerSize, fileTransmitPtr + multipacketReadDataPos, partSize);
            answerSize += partSize;
            multipacketReadDataPos += partSize;
            if (multipacketReadDataPos && (multipacketReadDataPos == fileTransmitSize))
                multipacketReadEnabled = false;
            answerCmd |= Cmd::MULTIPACKET_DATA_READ;
        }
    }
    if (cmdWord & Cmd::END_MULTIPACKET_READ)
    {
        multipacketReadEnabled = false;
    }
    if (cmdWord & Cmd::BEGIN_MULTIPACKET_WRITE)
    {
        if (multipacketWritePtr)
        {
            multipacketWritePtr->clear();
            multipacketWriteDataPos = 0;
            multipacketWriteSize = *(uint32_t*)(input + rxCnt);
            rxCnt += 4;
            multipacketCRC32 = *(uint32_t*)(input + rxCnt);
            rxCnt += 4;
            multipacketWriteEnabled = true;
            answerCmd |= Cmd::BEGIN_MULTIPACKET_WRITE;
        }
    }
    if (cmdWord & Cmd::MULTIPACKET_DATA_WRITE)
    {
        if (multipacketWriteEnabled && multipacketWritePtr)
        {
            uint16_t bufFreeSpace = inputSize - rxCnt - 2;
            uint16_t partSize = std::min<uint16_t>(*(uint16_t*)(input + rxCnt), bufFreeSpace);
            rxCnt += 2;
            for (int i = 0; i < partSize; i++)
                multipacketWritePtr->push_back(input[rxCnt + i]);
            answerCmd |= Cmd::MULTIPACKET_DATA_WRITE;
            multipacketWriteDataPos += partSize;
            printf("File write remainig bytes: %d\n", multipacketWriteSize - multipacketWriteDataPos);
        }
    }
    if (cmdWord & Cmd::END_MULTIPACKET_WRITE)
    {
        if (multipacketWritePtr)
        {
            if (multipacketCRC32 == crc32(multipacketWritePtr->data(), multipacketWritePtr->size(), true))
            {
                printf("File transfer success\n");
                answerCmd |= Cmd::END_MULTIPACKET_WRITE;
            }
            else
                printf("File transfer error\n");
            multipacketWritePtr = nullptr;
        }
        multipacketWriteEnabled = false;
    }
    *((unsigned int*)output) = answerCmd | Cmd::IS_ANSWER;
    return answerSize;
}

uint32_t RFDaemon::packParamData(uint8_t* dst, uint8_t dev, uint32_t dstSize)
{
    uint32_t offset = 0;
    *(uint16_t*)&dst[offset] = sizeof(parameters[0]) / sizeof(parameters[0][0]);
    offset += 2;

    for (uint32_t i = 0; i < parameters[dev].size(); i++)
    {
        if (dstSize >= (offset + parameters[dev][i].byteSize()))
        {
            memcpy(dst + offset, parameters[dev][i].name().data(), parameters[dev][i].name().size());
            offset += parameters[dev][i].name().size();
            dst[offset++] = 0;
            memcpy(dst + offset, parameters[dev][i].description().data(), parameters[dev][i].description().size());
            offset += parameters[dev][i].description().size();
            dst[offset++] = 0;
            memcpy(dst + offset, parameters[dev][i].unit().data(), parameters[dev][i].unit().size());
            offset += parameters[dev][i].unit().size();
            dst[offset++] = 0;
            dst[offset++] = parameters[dev][i].getType();
            *(int64_t*)(dst + offset) = parameters[dev][i].getValue();
            offset += 8;
            *(int64_t*)(dst + offset) = parameters[dev][i].getRangeMin();
            offset += 8;
            *(int64_t*)(dst + offset) = parameters[dev][i].getRangeMax();
            offset += 8;
        }
        else
            break;
    }
    return offset;
}

uint32_t RFDaemon::packParamData(uint8_t* dst, uint8_t dev, uint32_t dstSize, uint16_t paramId)
{
    uint32_t offset = 0;

    if (dstSize >= (offset + parameters[dev][paramId].byteSize()))
    {
        memcpy(dst + offset, parameters[dev][paramId].name().data(), parameters[dev][paramId].name().size());
        offset += parameters[dev][paramId].name().size();
        dst[offset++] = 0;
        memcpy(dst + offset, parameters[dev][paramId].description().data(), parameters[dev][paramId].description().size());
        offset += parameters[dev][paramId].description().size();
        dst[offset++] = 0;
        memcpy(dst + offset, parameters[dev][paramId].unit().data(), parameters[dev][paramId].unit().size());
        offset += parameters[dev][paramId].unit().size();
        dst[offset++] = 0;
        dst[offset++] = parameters[dev][paramId].getType();
        *(int64_t*)(dst + offset) = parameters[dev][paramId].getValue();
        offset += 8;
        *(int64_t*)(dst + offset) = parameters[dev][paramId].getRangeMin();
        offset += 8;
        *(int64_t*)(dst + offset) = parameters[dev][paramId].getRangeMax();
        offset += 8;
    }
    return offset;
}
