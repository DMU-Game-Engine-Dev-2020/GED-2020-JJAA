#pragma once

#include "rendering/material.h"

namespace Engine
{
	class OpenGLMaterial : public Material
	{
	private:
		std::shared_ptr<Shader> m_pShader;
		std::shared_ptr<VertexArray> m_pGeometry;
		std::map<std::string, void*> m_data;
	public:
		OpenGLMaterial(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& VAO);

		void setShader(const std::shared_ptr<Shader>& shader) override;
		void setGeometry(const std::variant<std::shared_ptr<VertexArray>, std::shared_ptr<VertexBuffer>>& geometry) override;
		void setDataBlock(const std::map<std::string, void*>& data) override;
		void setDataElement(const std::string& dataName, void* data) override;
		void setVertexData(float* vertices, unsigned int offset, unsigned int size) override;

		inline std::shared_ptr<Shader> getShader() override { return m_pShader; }
		inline std::variant<std::shared_ptr<VertexArray>, std::shared_ptr<VertexBuffer>> getGeometry() override { return m_pGeometry; }
		inline std::map<std::string, void*> getData() override { return m_data; }
	};
}