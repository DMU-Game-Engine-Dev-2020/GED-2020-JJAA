#include "engine_pch.h"

#include "platform/openGL/OpenGLIndexBuffer.h"
#include <glad/glad.h>

namespace Engine
{
	OpenGLIndexBuffer::OpenGLIndexBuffer(unsigned int* indices, unsigned int count) : m_iCount(count)
	{
		glCreateBuffers(1, &m_iRendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iRendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_iCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_iRendererID);
	}

	void OpenGLIndexBuffer::bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iRendererID);
	}

	void OpenGLIndexBuffer::unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}