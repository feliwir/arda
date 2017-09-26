#pragma once
#include "../layout.hpp"
#include "flextGL.h"

namespace arda
{
	class GLLayout final : public Layout
	{
	public:
		GLLayout();
		~GLLayout();

		// Geerbt �ber Layout
		virtual void Enable(int i) override;
		virtual void VertexPointer(int index, int size, Type type, int stride, void * offset) override;
		// Geerbt �ber Layout
		virtual void Bind() override;
	private:
		GLuint m_vao;
	};
}