/** \file OpenGLVertexBuffer.cpp
*/
#include "engine_pch.h"

#include "platform/openGL/OpenGLVertexBuffer.h"

#include <glad/glad.h>

namespace Engine
{
	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, unsigned int size, BufferLayout& layout) : m_layout(layout)
	{
		// Create the vertex buffer using the render ID as a name
		glCreateBuffers(1, &m_iRendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_iRendererID); // Bind the buffer
		// Initialize the data
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		// Delete the buffer
		glDeleteBuffers(1, &m_iRendererID);
	}

	void OpenGLVertexBuffer::bind()
	{
		// Bind the buffer
		glBindBuffer(GL_ARRAY_BUFFER, m_iRendererID);
	}

	void OpenGLVertexBuffer::unbind()
	{
		// Unbind the buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//void OpenGLVertexBuffer::edit(float * vertices, unsigned int size, unsigned int offset)
	//{
	//
	//}
}