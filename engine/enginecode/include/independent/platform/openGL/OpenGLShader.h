#pragma once

#include "rendering/shader.h"

#include <glad/glad.h>

namespace Engine
{
	class OpenGLShader : public Shader
	{
	private:
		unsigned int m_iShaderID;

		unsigned int m_iVertShader;
		unsigned int m_iFragShader;

		const char* m_pcVertSrc;
		const char* m_pcFragSrc;

		BufferLayout m_bufferLayout;
		UniformLayout m_uniformLayout;

		void parseSource(const std::string& filepath);
		void parseSource(const std::string& vertexFilepath, const std::string& fragmentFilepath);
		void extractBufferLayout(const std::string& line);
		void identifyUniform(const std::string& line);
		void storeUniformLocations();
		void compileAndLink(const std::string& vert, const std::string& frag);
		void dispatchUnifoermUpload(ShaderDataType type, GLuint location, void* data);
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& vertexFilepath, const std::string& fragmentFilepath);
		~OpenGLShader();

		inline unsigned int id() override { return m_iShaderID; }

		void bind() override;
		void unbind() override;

		bool uploadData(const std::string& name, void* data) override;
		bool uploadData(const UniformLayout& uniforms) override;

		inline BufferLayout getBufferLayout() const override { return m_bufferLayout; }
		inline UniformLayout getUniformLayout() const override { return m_uniformLayout; }
	};
}