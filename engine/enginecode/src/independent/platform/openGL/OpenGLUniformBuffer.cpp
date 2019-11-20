#include "engine_pch.h"

#include "platform/openGL/OpenGLUniformBuffer.h"

#include <glad/glad.h>

namespace Engine
{
	unsigned int OpenGLUniformBuffer::s_bindingPoint = 0;

	OpenGLUniformBuffer::OpenGLUniformBuffer(unsigned int size, UniformBufferLayout& layout) : m_layout(layout)
	{
		m_iBindingPoint = s_bindingPoint;
		s_bindingPoint++;

		glGenBuffers(1, &m_id);
		glBindBuffer(GL_UNIFORM_BUFFER, m_id);
		glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
		glBindBufferRange(GL_UNIFORM_BUFFER, m_iBindingPoint, m_id, 0, size);
	}

	OpenGLUniformBuffer::OpenGLUniformBuffer(unsigned int size, unsigned int rangeStart, unsigned int rangeEnd, UniformBufferLayout& layout) : m_layout(layout)
	{

	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		glDeleteBuffers(1, &m_id);
	}

	void OpenGLUniformBuffer::bind()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_id);
	}

	void OpenGLUniformBuffer::unbind()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void OpenGLUniformBuffer::attachShaderBlock(const std::shared_ptr<Shader>& shader, const std::string& blockName)
	{
		unsigned int uniformBlockIndex = glGetUniformBlockIndex(shader->id(), blockName.c_str());

		glUniformBlockBinding(shader->id(), uniformBlockIndex, m_iBindingPoint);
	}

	void OpenGLUniformBuffer::setData(unsigned int offset, unsigned int size, void* data)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_id);
		glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	}
}