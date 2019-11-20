#include "engine_pch.h"

#include "platform/openGL/OpenGLMaterial.h"

namespace Engine
{
	OpenGLMaterial::OpenGLMaterial(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& VAO)
	{
		m_pShader = shader;
		m_pGeometry = VAO;
	}

	void OpenGLMaterial::setShader(const std::shared_ptr<Shader>& shader)
	{
		m_pShader = shader;
	}

	void OpenGLMaterial::setGeometry(const std::variant<std::shared_ptr<VertexArray>, std::shared_ptr<VertexBuffer>>& geometry)
	{
		m_pGeometry = std::get<std::shared_ptr<VertexArray>>(geometry);
	}

	void OpenGLMaterial::setDataBlock(const std::map<std::string, void*>& data)
	{
		m_data.insert(data.begin(), data.end());
	}

	void OpenGLMaterial::setDataElement(const std::string& dataName, void* data)
	{
		m_data.insert(std::make_pair(dataName, data));
	}

	/*void OpenGLMaterial::setVertexData(float* vertices, unsigned int offset, unsigned int size)
	{

	}*/
}