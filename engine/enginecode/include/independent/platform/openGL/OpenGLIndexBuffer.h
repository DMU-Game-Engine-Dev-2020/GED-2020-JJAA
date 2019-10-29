#pragma once

#include "rendering/indexBuffer.h"

namespace Engine
{
	class OpenGLIndexBuffer : public IndexBuffer
	{
	private:
		unsigned int m_iRendererID;
		unsigned int m_iCount;
	public:
		OpenGLIndexBuffer(unsigned int* indices, unsigned int count);
		~OpenGLIndexBuffer();

		void bind() override;
		void unbind() override;

		inline unsigned int getCount() const override { return m_iCount; }
	};
}