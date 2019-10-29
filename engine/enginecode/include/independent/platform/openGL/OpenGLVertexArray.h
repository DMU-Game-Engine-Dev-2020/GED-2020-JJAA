#pragma once

#include "rendering/vertexArray.h"

namespace Engine
{
	class OpenGLVertexArray : public VertexArray
	{
	private:
		unsigned int m_iRendererID;
		unsigned int m_iDrawCount;

		std::shared_ptr<VertexBuffer> m_vertexBuffer;
		std::shared_ptr<IndexBuffer> m_indexBuffer;
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		void bind() override;
		void unbind() override;
		void setVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		void setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

		std::shared_ptr<VertexBuffer> getVertexBuffer() const override { return m_vertexBuffer; }
		std::shared_ptr<IndexBuffer> getIndexBuffer() const override { return m_indexBuffer; }

		inline unsigned int getDrawCount() const override { return m_iDrawCount; }
	};
}