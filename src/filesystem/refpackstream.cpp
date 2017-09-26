#include "refpackstream.hpp"
#include "util.hpp"
#include "../core/debugger.hpp"
#include <iostream>
#include <algorithm>

arda::RefPackStream::RefPackStream(std::shared_ptr<IStream> compressedstream) :
	m_currentOutputPosition(0),
	m_nextOutputPosition(0),
	m_offset(0)
{
	m_eof = false;
	m_stream = compressedstream;

	uint8_t headerByte1 = util::Read<uint8_t>(m_stream);

	if ((headerByte1 & 0b00111110) != 0b00010000)
	{
		ARDA_LOG("Invalid refpackstream");
		return;
	}

	uint8_t largeFilesFlag = (headerByte1 & 0b1000000) != 0;
	uint8_t compressedSizePresent = (headerByte1 & 0b00000001) != 0;
	uint8_t headerByte2 = util::Read<uint8_t>(m_stream);

	if (headerByte2 != 0b11111011)
	{
		ARDA_LOG("Invalid refpackstream");
		return;
	}

	int count = largeFilesFlag ? 4 : 3;
	if (compressedSizePresent)
	{
	    int compressedSize = ReadBigEndianSize(count);
		int i = 0;
	}
	int decompressedSize = ReadBigEndianSize(count);

	m_output.resize(decompressedSize);
}

void arda::RefPackStream::Seek(int offset, SeekOrigin origin)
{
	
}

unsigned int arda::RefPackStream::Read(char *buffer, size_t numBytes)
{
	int actualCount = std::min(numBytes, m_output.size() - m_currentOutputPosition);

	while (!m_eof && m_currentOutputPosition + actualCount > m_nextOutputPosition)
	{
		ExecuteCommand();
	}
	for (int i = 0; i < actualCount; i++)
	{
		buffer[i] = m_output[m_currentOutputPosition++];
	}
	return actualCount;
}

void arda::RefPackStream::ExecuteCommand()
{
	uint8_t byte1 = util::Read<uint8_t>(m_stream);
	if ((byte1 & 0x80) == 0) 
		Execute2ByteCommand(byte1);

	else if ((byte1 & 0x40) == 0)
		Execute3ByteCommand(byte1);

	else if ((byte1 & 0x20) == 0) 
		Execute4ByteCommand(byte1);
	else // 1-byte command
	{
		if (byte1 < 0xFC) // Ordinary 1-byte command
			Execute1ByteCommand(byte1);
		else // Stop command
			Execute1ByteCommandAndStop(byte1);
	}
}

void arda::RefPackStream::Execute1ByteCommand(int byte1)
{
	int proceedingDataLength = ((byte1 & 0x1F) + 1) << 2;
	CopyProceeding(proceedingDataLength);
}

void arda::RefPackStream::Execute2ByteCommand(int byte1) 
{
	uint8_t byte2 = util::Read<uint8_t>(m_stream);
	uint8_t proceedingDataLength = byte1 & 0x03;

	CopyProceeding(proceedingDataLength);
	int referencedDataLength = ((byte1 & 0x1C) >> 2) + 3;
	int referencedDataDistance = ((byte1 & 0x60) << 3) + byte2 + 1;
	CopyReferencedData(referencedDataLength, referencedDataDistance);
}

void arda::RefPackStream::Execute3ByteCommand(int byte1) 
{
	uint8_t byte2 = util::Read<uint8_t>(m_stream);
	uint8_t byte3 = util::Read<uint8_t>(m_stream);
	int proceedingDataLength = (byte2 & 0xC0) >> 6;

	CopyProceeding(proceedingDataLength);

	int referencedDataLength = (byte1 & 0x3F) + 4;
	int referencedDataDistance = ((byte2 & 0x3F) << 8) + byte3 + 1;

	CopyReferencedData(referencedDataLength, referencedDataDistance);
}

void arda::RefPackStream::Execute4ByteCommand(int byte1)
{
	uint8_t byte2 = util::Read<uint8_t>(m_stream);
	uint8_t byte3 = util::Read<uint8_t>(m_stream);
	uint8_t byte4 = util::Read<uint8_t>(m_stream);

	int proceedingDataLength = byte1 & 0x03;
	CopyProceeding(proceedingDataLength);

	int referencedDataLength = ((byte1 & 0x0C) << 6) + byte4 + 5;
	int referencedDataDistance = ((byte1 & 0x10) << 12) + (byte2 << 8) + byte3 + 1;

	CopyReferencedData(referencedDataLength, referencedDataDistance);
}

void arda::RefPackStream::Execute1ByteCommandAndStop(int byte1)
{
	int proceedingDataLength = byte1 & 0x03;

	CopyProceeding(proceedingDataLength);
	m_eof = true;
}

void arda::RefPackStream::CopyProceeding(int proceedingDataLength)
{
	while (proceedingDataLength-- > 0)
	{
		m_output[m_nextOutputPosition++] = util::Read<uint8_t>(m_stream);
	}
}

void arda::RefPackStream::CopyReferencedData(int referencedDataLength, int referencedDataDistance)
{
	auto referencedDataIndex = m_nextOutputPosition - referencedDataDistance;
	while (referencedDataLength-- > 0)
	{
		m_output[m_nextOutputPosition++] = m_output[referencedDataIndex++];
	}
}

int arda::RefPackStream::ReadBigEndianSize(int count)
{
	int size = 0;
	for (int i = 0; i < count; i++)
	{
		size = (size << 8) | util::Read<uint8_t>(m_stream);
	}
	return size;
}