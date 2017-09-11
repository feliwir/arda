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
		virtual void Seek(int offset, SeekOrigin origin) override;
		virtual unsigned int Read(char *buffer, size_t numBytes) override;

		inline int GetOffset() { return m_offset; }

	private:
		void ExecuteCommand();
		void Execute1ByteCommand(int byte1);
		void Execute2ByteCommand(int byte1);
		void Execute3ByteCommand(int byte1);
		void Execute4ByteCommand(int byte1);
		void Execute1ByteCommandAndStop(int byte1);
		void CopyProceeding(int proceedingDataLength);
		void CopyReferencedData(int referencedDataLength, int referencedDataDistance);
		int ReadBigEndianSize(int count);

		std::shared_ptr<IStream> m_stream;
		std::vector<uint8_t> m_output;
		int m_currentOutputPosition;
		int m_nextOutputPosition;
		bool m_eof;

	protected:
		int m_offset;

	};
}