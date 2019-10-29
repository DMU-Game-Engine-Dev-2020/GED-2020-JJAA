#include "engine_pch.h"

#include "platform/openGL/OpenGLVertexBuffer.h"

#include <glad/glad.h>

namespace Engine
{
	OpenGLVertexBuffer::OpenGLVertexBuffer(float * vertices, unsigned int size, BufferLayout& layout) : m_layout(layout)
	{
		glCreateBuffers(1, &m_iRendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_iRendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_iRendererID);
	}

	void OpenGLVertexBuffer::bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_iRendererID);
	}

	void OpenGLVertexBuffer::unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::edit(float * vertices, unsigned int size, unsigned int offset)
	{

	}
}