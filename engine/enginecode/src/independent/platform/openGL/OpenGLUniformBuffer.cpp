/** \file OpenGLUniformBuffer.cpp
*/
#include "engine_pch.h"

#include "platform/openGL/OpenGLUniformBuffer.h"

#include <glad/glad.h>

namespace Engine
{
	unsigned int OpenGLUniformBuffer::s_bindingPoint = 0; // Initialize the static variable

	OpenGLUniformBuffer::OpenGLUniformBuffer(unsigned int size, UniformBufferLayout& layout) : m_layout(layout)
	{
		m_iBindingPoint = s_bindingPoint; // Set the binding point to the next one available
		s_bindingPoint++; // Increase the static binding point for the next uniform buffer

		glGenBuffers(1, &m_id); // Create the buffer
		glBindBuffer(GL_UNIFORM_BUFFER, m_id); // Bind the buffer
		glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_DYNAMIC_DRAW); // Create the data store
		glBindBufferRange(GL_UNIFORM_BUFFER, m_iBindingPoint, m_id, 0, size);
	}

	OpenGLUniformBuffer::OpenGLUniformBuffer(unsigned int size, unsigned int rangeStart, unsigned int rangeEnd, UniformBufferLayout& layout) : m_layout(layout)
	{

	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		glDeleteBuffers(1, &m_id); // Delete the buffer
	}

	void OpenGLUniformBuffer::bind()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_id); // Bind the buffer
	}

	void OpenGLUniformBuffer::unbind()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, 0); // Unbind the buffer
	}

	void OpenGLUniformBuffer::attachShaderBlock(const std::shared_ptr<Shader>& shader, const std::string& blockName)
	{
		// Get the block index using the shader id and the name of the block
		unsigned int uniformBlockIndex = glGetUniformBlockIndex(shader->id(), blockName.c_str());
		// Link the uniform block index to the binding point
		glUniformBlockBinding(shader->id(), uniformBlockIndex, m_iBindingPoint);
	}

	void OpenGLUniformBuffer::setData(unsigned int offset, unsigned int size, void* data)
	{
		// Bind the buffer
		glBindBuffer(GL_UNIFORM_BUFFER, m_id);
		glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data); // Update the buffer data
	}
}