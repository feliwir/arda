#pragma once
#include "../buffer.hpp"

namespace arda
{
	class D3DBuffer : public Buffer
	{
	public:
		D3DBuffer(Type t, Usage u);

		// Geerbt über Buffer
		virtual void Upload(uint8_t * data, int bufSize) override;
		virtual void Bind() override;
	};
}