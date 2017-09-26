#pragma once
#include "../layout.hpp"

namespace arda
{
	class D3DLayout final : public Layout
	{
	public:
		// Geerbt über Layout
		virtual void Bind() override;
		virtual void Enable(int i) override;
		virtual void VertexPointer(int index, int size, Type type, int stride, void * offset) override;
	};
}