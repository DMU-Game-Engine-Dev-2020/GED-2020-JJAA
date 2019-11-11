/** \file OpenGLShader.h
*/
#pragma once

#include "rendering/shader.h"

#include <glad/glad.h>

namespace Engine
{
	/**
	\class OpenGLShader
	\brief OpenGL specific shader
	*/
	class OpenGLShader : public Shader
	{
	private:
		unsigned int m_iShaderID; //!< The shader ID

		unsigned int m_iVertShader; //!< The reference to the vertex shader
		unsigned int m_iFragShader; //!< The reference to the fragment shader

		const char* m_pcVertSrc; //!< The vertex shader
		const char* m_pcFragSrc; //!< The fragment shader

		BufferLayout m_bufferLayout; //!< The buffer layout

		//Maybe
		//UniformLayout2 m_uniformLayout2;
		UniformLayout m_uniformLayout; //!< The uniform layout

		//! Function to parse a shader source file
		/*!
		\param filepath The location that the file is saved
		*/
		void parseSource(const std::string& filepath);
		//! Function to parse separate vertex and fragment shader files
		/*!
		\param vertexFilepath The location that the vertex shader file is saved
		\param fragmentFilepath The location that the fragment shader file is saved
		*/
		void parseSource(const std::string& vertexFilepath, const std::string& fragmentFilepath);
		//! Function to extract the buffer layout from the shader file(s)
		/*!
		\param line The line being checked for a buffer variable
		*/
		void extractBufferLayout(const std::string& line);
		//! Function to extract the uniform layout from the shader file(s)
		/*!
		\param line The line being checked for a uniform variable
		*/
		void identifyUniform(const std::string& line);
		//! Function to cache the uniform locations
		void storeUniformLocations();
		//! Function to compile and link the shader
		/*!
		\param vert String with the vertex shader code
		\param frag String with the fragment shader code
		*/
		void compileAndLink(const std::string& vert, const std::string& frag);

		//! Function to upload uniform data
		/*!
		\param type The data type
		\param location The location of the uniform
		\param data Pointer to the data
		*/
		void dispatchUnifoermUpload(ShaderDataType type, GLuint location, void* data);
	public:
		//! Constructor
		/*!
		\param filepath The filepath for where the shader file is saved
		*/
		OpenGLShader(const std::string& filepath);
		//! Constructor
		/*!
		\param vertexFilepath The filepath for where the vertex shader file is saved
		\param fragmentFilepath The filepath for where the fragment shader file is saved
		*/
		OpenGLShader(const std::string& vertexFilepath, const std::string& fragmentFilepath);
		//! Destructor
		~OpenGLShader();

		//! Function to get the shader ID
		/*!
		\return The shader ID
		*/
		inline unsigned int id() override { return m_iShaderID; }

		void bind() override; //!< Bind the shader
		void unbind() override; //!< Unbind the shader

		//! Function to upload data to the GPU
		/*!
		\param name The name of the uniform being uploaded
		\param data The data being uploaded
		*/
		bool uploadData(const std::string& name, void* data) override;
		//! Function to upload data to the GPU
		/*!
		\param uniforms The uniform data
		*/
		bool uploadData(const UniformLayout& uniforms) override;

		//! Function to get the buffer layout
		/*!
		\return The buffer layout
		*/
		inline BufferLayout getBufferLayout() const override { return m_bufferLayout; }
		//! Function to get the uniform layout
		/*!
		\return The uniform layout
		*/
		inline UniformLayout getUniformLayout() const override { return m_uniformLayout; }
	};
}