#pragma once

namespace arda
{
    class Layout
    {
	public:
		enum Type
		{
			FLOAT,
			UBYTE,
			BYTE,
		};

		virtual void Bind() = 0;

		virtual void Enable(int i) = 0;

		virtual void VertexPointer(int index, int size, Type type, int stride, void* offset) = 0;
    };
}