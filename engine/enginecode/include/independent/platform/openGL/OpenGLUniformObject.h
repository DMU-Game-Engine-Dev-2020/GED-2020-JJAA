#pragma once

#include "rendering/uniformObject.h"

#include <glad/glad.h>

namespace Engine
{
	class OpenGLUniformObject : public UniformObject
	{
	private:
		std::string m_sName;
		ShaderDataType m_type;
		GLuint m_location;

		UploadFunc m_func;
	public:
		OpenGLUniformObject(std::string& name, ShaderDataType type) : m_sName(name), m_type(type) {}

		void setLocationAndFunction(unsigned int shaderID, UploadFunc& func) override
		{
			m_location = glGetUniformLocation(shaderID, m_sName.c_str());
			m_func = func;
		}

		inline ShaderDataType getType() override { return m_type; }
		inline void uniformUpload(void* data) override { m_func(m_location, data); }
	};
}