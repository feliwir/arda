#include "refpackstream.hpp"
#include "../filesystem/util.hpp"
#include <iostream>
#include <algorithm>

arda::RefPackStream::RefPackStream(std::shared_ptr<IStream> compressedstream)
{
	m_eof = false;
	m_stream = compressedstream;
	std::cout << "loading map " << std::endl;
	uint8_t headerByte1 = util::read<uint8_t>(m_stream);
	if ((headerByte1 & 0b00111110) != 0b00010000)
	{
		std::cout << "invalid map data " << std::endl;
		return;
	}

	uint8_t largeFilesFlag = (headerByte1 & 0b1000000) != 0;
	uint8_t compressedSizePresent = (headerByte1 & 0b00000001) != 0;
	uint8_t headerByte2 = util::read<uint8_t>(m_stream);

	if (headerByte2 != 0b11111011)
	{
		std::cout << "invalid map data " << std::endl;
		return;
	}

	int count = largeFilesFlag ? 4 : 3;
	if (compressedSizePresent)
	{
	    int compressedSize = readBigEndianSize(count);
		int i = 0;
	}
	int decompressedSize = readBigEndianSize(count);

	m_output.resize(decompressedSize);
}

void arda::RefPackStream::seek(int offset, SeekOrigin origin)
{
	
}

unsigned int arda::RefPackStream::read(char *buffer, size_t numBytes)
{
	int actualCount = std::min(numBytes, m_output.size() - m_currentOutputPosition);

	while (!m_eof && m_currentOutputPosition + actualCount > m_nextOutputPosition)
	{
		executeCommand();
	}
	for (int i = 0; i < actualCount; i++)
	{
		buffer[i] = m_output[m_currentOutputPosition++];
	}
	return actualCount;
}

void arda::RefPackStream::executeCommand()
{
	uint8_t byte1 = util::read<uint8_t>(m_stream);
	if ((byte1 & 0x80) == 0) 
		execute2ByteCommand(byte1);

	else if ((byte1 & 0x40) == 0)
		execute3ByteCommand(byte1);

	else if ((byte1 & 0x20) == 0) 
		execute4ByteCommand(byte1);
	else // 1-byte command
	{
		if (byte1 < 0xFC) // Ordinary 1-byte command
			execute1ByteCommand(byte1);
		else // Stop command
			execute1ByteCommandAndStop(byte1);
	}
}

void arda::RefPackStream::execute1ByteCommand(int byte1)
{
	int proceedingDataLength = ((byte1 & 0x1F) + 1) << 2;
	copyProceeding(proceedingDataLength);
}

void arda::RefPackStream::execute2ByteCommand(int byte1) 
{
	uint8_t byte2 = util::read<uint8_t>(m_stream);
	uint8_t proceedingDataLength = byte1 & 0x03;

	copyProceeding(proceedingDataLength);
	int referencedDataLength = ((byte1 & 0x1C) >> 2) + 3;
	int referencedDataDistance = ((byte1 & 0x60) << 3) + byte2 + 1;
	copyReferencedData(referencedDataLength, referencedDataDistance);
}

void arda::RefPackStream::execute3ByteCommand(int byte1) 
{
	uint8_t byte2 = util::read<uint8_t>(m_stream);
	uint8_t byte3 = util::read<uint8_t>(m_stream);
	int proceedingDataLength = (byte2 & 0xC0) >> 6;

	copyProceeding(proceedingDataLength);

	int referencedDataLength = (byte1 & 0x3F) + 4;
	int referencedDataDistance = ((byte2 & 0x3F) << 8) + byte3 + 1;

	copyReferencedData(referencedDataLength, referencedDataDistance);
}

void arda::RefPackStream::execute4ByteCommand(int byte1)
{
	uint8_t byte2 = util::read<uint8_t>(m_stream);
	uint8_t byte3 = util::read<uint8_t>(m_stream);
	uint8_t byte4 = util::read<uint8_t>(m_stream);

	int proceedingDataLength = byte1 & 0x03;
	copyProceeding(proceedingDataLength);

	int referencedDataLength = ((byte1 & 0x0C) << 6) + byte4 + 5;
	int referencedDataDistance = ((byte1 & 0x10) << 12) + (byte2 << 8) + byte3 + 1;

	copyReferencedData(referencedDataLength, referencedDataDistance);
}

void arda::RefPackStream::execute1ByteCommandAndStop(int byte1)
{
	int proceedingDataLength = byte1 & 0x03;

	copyProceeding(proceedingDataLength);
	m_eof = true;
}

void arda::RefPackStream::copyProceeding(int proceedingDataLength)
{
	while (proceedingDataLength-- > 0)
	{
		m_output[m_nextOutputPosition++] = util::read<uint8_t>(m_stream);
	}
}

void arda::RefPackStream::copyReferencedData(int referencedDataLength, int referencedDataDistance)
{
	auto referencedDataIndex = m_nextOutputPosition - referencedDataDistance;
	while (referencedDataLength-- > 0)
	{
		m_output[m_nextOutputPosition++] = m_output[referencedDataIndex++];
	}
}

int arda::RefPackStream::readBigEndianSize(int count)
{
	int size = 0;
	for (int i = 0; i < count; i++)
	{
		size = (size << 8) | util::read<uint8_t>(m_stream);
	}
	return size;
}