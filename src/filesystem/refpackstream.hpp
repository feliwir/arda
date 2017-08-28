#pragma once
#include "stream.hpp"
#include <memory>
#include <vector>


namespace arda
{
	class RefPackStream : public IStream
	{
	public:
		RefPackStream(std::shared_ptr<IStream> compressedstream);

		// Inherited via IStream
		virtual void seek(int offset, SeekOrigin origin) override;
		virtual unsigned int read(char *buffer, size_t numBytes) override;

		inline int getOffset() { return m_offset; }

	private:
		void executeCommand();
		void execute1ByteCommand(int byte1);
		void execute2ByteCommand(int byte1);
		void execute3ByteCommand(int byte1);
		void execute4ByteCommand(int byte1);
		void execute1ByteCommandAndStop(int byte1);
		void copyProceeding(int proceedingDataLength);
		void copyReferencedData(int referencedDataLength, int referencedDataDistance);
		int readBigEndianSize(int count);

		std::shared_ptr<IStream> m_stream;
		std::vector<uint8_t> m_output;
		int m_currentOutputPosition;
		int m_nextOutputPosition;
		bool m_eof;

	protected:
		int m_offset;

	};
}