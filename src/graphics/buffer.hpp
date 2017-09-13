#pragma once
#include <vector>
#include <stdint.h>

namespace arda
{
	class Buffer
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
	
		Buffer(Type type,Usage usage);
		
		template<class T>
		inline void Upload(std::vector<T> data)
		{
			const int el_size = sizeof(T);
			Upload(data.data(), data.size()*el_size);
		}

		virtual void Upload(uint8_t* data, int bufSize) = 0;
		virtual void Bind() = 0;

	protected:
		Type m_type;
		Usage m_usage;
		int m_byteSize;
	};
}