/** \file OpenGLVertexArray.h
*/
#pragma once

#include "rendering/vertexArray.h"

namespace Engine
{
	/**
	\class OpenGLVertexArray
	\brief Open GL specific VertexArray
	*/
	class OpenGLVertexArray : public VertexArray
	{
	private:
		unsigned int m_iRendererID; //!< Name for vertex array creation and binding
		unsigned int m_iDrawCount; //!< The number of triangles to draw

		std::shared_ptr<VertexBuffer> m_pVertexBuffer; //!< Pointer to a VertexBuffer class
		std::shared_ptr<IndexBuffer> m_pIndexBuffer; //!< Pointer to an IndexBuffer class
	public:
		OpenGLVertexArray(); //!< Constructor
		~OpenGLVertexArray(); //!< Destructor

		void bind() override; //!< Bind the vertex array object
		void unbind() override; //!< Unind the vertex array object
		void setVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override; //!< Add and set the vertex buffer
		void setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override; //!< Set the index buffer

		std::shared_ptr<VertexBuffer> getVertexBuffer() const override { return m_pVertexBuffer; } //!< Returns the pointer to the vertex buffer
		std::shared_ptr<IndexBuffer> getIndexBuffer() const override { return m_pIndexBuffer; } //!< Returns the pointer to the index buffer

		inline unsigned int getDrawCount() const override { return m_iDrawCount; } //!< Returns the draw count
	};
}