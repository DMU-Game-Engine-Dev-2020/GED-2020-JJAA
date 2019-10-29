#pragma once

#include "rendering/vertexBuffer.h"

namespace Engine
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	private:
		unsigned int m_iRendererID;
		BufferLayout m_layout;
	public:
		OpenGLVertexBuffer(float* vertices, unsigned int size, BufferLayout& layout);
		~OpenGLVertexBuffer();

		void bind() override;
		void unbind() override;

		void edit(float* vertices, unsigned int size, unsigned int offset) override;
		const BufferLayout& getLayout() const override { return m_layout; }
	};
}