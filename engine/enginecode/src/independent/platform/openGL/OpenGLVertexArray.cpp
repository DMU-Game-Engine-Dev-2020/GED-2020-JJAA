#include "engine_pch.h"

#include "platform/openGL/OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Engine
{
	static GLenum ShaderDataTypeToOpenGLType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Int:        return GL_INT;
		case ShaderDataType::Int2:       return GL_INT;
		case ShaderDataType::Int3:       return GL_INT;
		case ShaderDataType::Int4:       return GL_INT;
		case ShaderDataType::Float:      return GL_FLOAT;
		case ShaderDataType::Float2:     return GL_FLOAT;
		case ShaderDataType::Float3:     return GL_FLOAT;
		case ShaderDataType::Float4:     return GL_FLOAT;
		case ShaderDataType::Mat3:       return GL_FLOAT;
		case ShaderDataType::Mat4:       return GL_FLOAT;
		case ShaderDataType::Bool:       return GL_BOOL;
		}
		return 0;
	}


	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_iRendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_iRendererID);
	}

	void OpenGLVertexArray::bind()
	{
		glBindVertexArray(m_iRendererID);
	}

	void OpenGLVertexArray::unbind()
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::setVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		glBindVertexArray(m_iRendererID);
		vertexBuffer->bind();

		BufferLayout layout = vertexBuffer->getLayout();

		unsigned int index = 0;
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(
				index,
				ShaderDataTypeComponentCount(element.m_dataType),
				ShaderDataTypeToOpenGLType(element.m_dataType),
				element.m_bNormalized ? GL_TRUE : GL_FALSE,
				layout.getStride(),
				(const void*)element.m_iOffset);
			index++;
		}

		m_vertexBuffer = vertexBuffer;
	}

	void OpenGLVertexArray::setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_iRendererID);
		indexBuffer->bind();

		m_indexBuffer = indexBuffer;
	}

}