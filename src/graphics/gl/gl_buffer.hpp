#pragma once
#include "../buffer.hpp"
#include "flextGL.h"

namespace arda
{
	class GLBuffer final : public IBuffer
	{
	public:
		GLBuffer(Type t,Usage u);
		~GLBuffer();

		// Inherited from Buffer
		virtual void Upload(uint8_t * data, int bufSize) override;
		virtual void Bind() override;

	private:
		void Generate();
		GLenum getTarget();
		GLenum getUsage();
	private:
		GLenum m_target;
		GLenum m_usageFlag;
		GLuint m_handle;
	};
}