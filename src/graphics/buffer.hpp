#pragma once
#include <vector>
#include <stdint.h>

namespace arda
{
	class IBuffer
	{
	public:
		enum Type
		{
			VERTEX_BUFFER,
			INDEX_BUFFER
		};

		enum Usage
		{
			STATIC,
			DYNAMIC,
			STREAM
		};
	
		IBuffer(Type type,Usage usage);
		
		template<class T>
		inline void Upload(std::vector<T> buffer)
		{
			m_numElems = buffer.size();
			const int el_size = sizeof(T);
			m_byteSize = el_size * m_numElems;
			uint8_t* buf = reinterpret_cast<uint8_t*>(buffer.data());
			Upload(buf, m_byteSize);
		}

		virtual void Upload(uint8_t* data, int bufSize) = 0;
		virtual void Bind() = 0;

		inline int GetElementNumber()
		{
			return m_numElems;
		}

		inline int GetByteSize()
		{
			return m_byteSize;
		}
	protected:
		Type m_type;
		Usage m_usage;
		int m_numElems;
		int m_byteSize;
	};
}