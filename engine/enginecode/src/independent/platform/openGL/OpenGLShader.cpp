#include "engine_pch.h"

#include "platform/openGL/OpenGLShader.h"
#include "systems/log.h"

//#include <glad/glad.h>
#include <glm/glm.hpp>
#include <fstream>
#include <sstream>

namespace Engine
{
	static ShaderDataType GLSLStrToSDT(const std::string& glslSrc)
	{
		ShaderDataType               result = ShaderDataType::None;

		if (glslSrc == "int")        result = ShaderDataType::Int;
		if (glslSrc == "ivec2")      result = ShaderDataType::Int2;
		if (glslSrc == "ivec3")      result = ShaderDataType::Int3;
		if (glslSrc == "ivec4")      result = ShaderDataType::Int4;
		if (glslSrc == "float")      result = ShaderDataType::Float;
		if (glslSrc == "vec2")       result = ShaderDataType::Float2;
		if (glslSrc == "vec3")       result = ShaderDataType::Float3;
		if (glslSrc == "vec4")       result = ShaderDataType::Float4;
		if (glslSrc == "mat3")       result = ShaderDataType::Mat3;
		if (glslSrc == "mat4")       result = ShaderDataType::Mat4;
		if (glslSrc == "bool")       result = ShaderDataType::Bool;
		if (glslSrc == "sampler2D")  result = ShaderDataType::Sampler2D;

		return result;
	}


	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		m_iVertShader = glCreateShader(GL_VERTEX_SHADER);
		m_iFragShader = glCreateShader(GL_FRAGMENT_SHADER);

		parseSource(filepath);

		storeUniformLocations();
	}

	OpenGLShader::OpenGLShader(const std::string& vertexFilepath, const std::string& fragmentFilepath)
	{
		m_iVertShader = glCreateShader(GL_VERTEX_SHADER);
		m_iFragShader = glCreateShader(GL_FRAGMENT_SHADER);

		parseSource(vertexFilepath, fragmentFilepath);

		storeUniformLocations();
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_iShaderID);
	}

	void OpenGLShader::bind()
	{
		glUseProgram(m_iShaderID);
	}

	void OpenGLShader::unbind()
	{
		glUseProgram(0);
	}

	bool OpenGLShader::uploadData(const std::string& name, void* data)
	{
		std::pair<ShaderDataType, int> typeAndLocation;

		UniformLayout::iterator it; 

		it = m_uniformLayout.find(name);

		if (it == m_uniformLayout.end())
		{
			LOG_ERROR("No uniform variable with name: {0} found in shaders", name);
		}
		else
		{
			typeAndLocation = it->second;
		}

		GLuint location = (GLuint)typeAndLocation.second;

		//GLuint loc = glGetUniformLocation(m_iShaderID, name.c_str());
		dispatchUnifoermUpload(typeAndLocation.first, location, data);
		return true;
	}

	bool OpenGLShader::uploadData(const UniformLayout& uniforms)
	{
		m_uniformLayout = uniforms;
		return true;
	}

	void OpenGLShader::parseSource(const std::string& filepath)
	{
		std::fstream handle(filepath, std::ios::in);
		enum { NONE = -1, VERTEX = 0, FRAGMENT } region;

		if (!handle.is_open()) 
			LOG_CRITICAL("Could not open shader file '{0}'", filepath);

		std::string line;
		std::string src[2];

		while (getline(handle, line))
		{
			if (line.find("#region Vertex") != std::string::npos) { region = VERTEX; continue; }
			if (line.find("#region Fragment") != std::string::npos) { region = FRAGMENT; continue; }
			if (line.find("#region Geometry") != std::string::npos) { region = NONE; continue; }
			if (line.find("#region Tessalation") != std::string::npos) { region = NONE; continue; }

			if (region == VERTEX && line.find("in ") != std::string::npos)
			{
				extractBufferLayout(line);
			}

			if (region != NONE)
			{
				if (line.find("uniform") != std::string::npos)
				{
					identifyUniform(line);
				}

				src[region] += (line + "\n");
			}
		}
		
		handle.close();

		compileAndLink(src[VERTEX], src[FRAGMENT]);
	}

	void OpenGLShader::parseSource(const std::string& vertexFilepath, const std::string& fragmentFilepath)
	{
		std::fstream vertexHandle(vertexFilepath, std::ios::in);
		enum { NONE = -1, VERTEX = 0, FRAGMENT };

		if (!vertexHandle.is_open())
			LOG_CRITICAL("Could not open shader file '{0}'", vertexFilepath);

		std::string line;
		std::string src[2];

		while (getline(vertexHandle, line))
		{
			if (line.find("in ") != std::string::npos)
			{
				extractBufferLayout(line);
			}
			if (line.find("uniform") != std::string::npos)
			{
				identifyUniform(line);
			}
			src[VERTEX] += (line + "\n");
		}

		vertexHandle.close();


		std::fstream fragmentHandle(fragmentFilepath, std::ios::in);

		if (!fragmentHandle.is_open())
			LOG_CRITICAL("Could not open shader file '{0}'", fragmentFilepath);

		while (getline(fragmentHandle, line))
		{
			if (line.find("uniform") != std::string::npos)
			{
				identifyUniform(line);
			}
			src[FRAGMENT] += (line + "\n");
		}

		fragmentHandle.close();

		compileAndLink(src[VERTEX], src[FRAGMENT]);
	}

	void OpenGLShader::extractBufferLayout(const std::string& line)
	{
		std::string type;

		std::stringstream iss(line);
		bool isType = false;

		while (iss >> type)
		{
			if (!isType)
			{
				if (type == "in")
				{
					isType = true;
				}
			}
			else
			{
				break;
			}
		}

		m_bufferLayout.addElement(GLSLStrToSDT(type));
	}

	void OpenGLShader::identifyUniform(const std::string& line)
	{
		std::string type;
		std::string name;

		std::stringstream iss(line);

		bool isType = false;

		while (iss >> type)
		{
			if (!isType)
			{
				if (type == "uniform")
				{
					isType = true;
				}
			}
			else
			{
				iss >> name;
				name.erase(std::remove(name.begin(), name.end(), ';'), name.end());
				break;
			}
		}
		m_uniformLayout.insert(std::make_pair(name, std::make_pair(GLSLStrToSDT(type), 0)));
	}

	void OpenGLShader::storeUniformLocations()
	{
		for (const auto& it : m_uniformLayout)
		{
			const std::string name = it.first;
			GLuint location = glGetUniformLocation(m_iShaderID, name.c_str());

			m_uniformLayout.find(name)->second.second = (int)location;
		}
	}

	void OpenGLShader::compileAndLink(const std::string& vert, const std::string& frag)
	{
		m_pcVertSrc = vert.c_str();

		glShaderSource(m_iVertShader, 1, &m_pcVertSrc, 0);
		glCompileShader(m_iVertShader);

		GLint m_isCompiled = 0;
		glGetShaderiv(m_iVertShader, GL_COMPILE_STATUS, &m_isCompiled);
		if (m_isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(m_iVertShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(m_iVertShader, maxLength, &maxLength, &infoLog[0]);
			LOG_ERROR("Shader compile error: {0}", std::string(infoLog.begin(), infoLog.end()));

			glDeleteShader(m_iVertShader);
			return;
		}

		m_pcFragSrc = frag.c_str();
		glShaderSource(m_iFragShader, 1, &m_pcFragSrc, 0);
		glCompileShader(m_iFragShader);

		glGetShaderiv(m_iFragShader, GL_COMPILE_STATUS, &m_isCompiled);
		if (m_isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(m_iFragShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(m_iFragShader, maxLength, &maxLength, &infoLog[0]);
			LOG_ERROR("Shader compile error: {0}", std::string(infoLog.begin(), infoLog.end()));

			glDeleteShader(m_iFragShader);
			glDeleteShader(m_iVertShader);

			return;
		}


		m_iShaderID = glCreateProgram();
		glAttachShader(m_iShaderID, m_iVertShader);
		glAttachShader(m_iShaderID, m_iFragShader);
		glLinkProgram(m_iShaderID);

		GLint m_isLinked = 0;
		glGetProgramiv(m_iShaderID, GL_LINK_STATUS, (int*)&m_isLinked);
		if (m_isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_iShaderID, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_iShaderID, maxLength, &maxLength, &infoLog[0]);
			LOG_ERROR("Shader linking error: {0}", std::string(infoLog.begin(), infoLog.end()));

			glDeleteProgram(m_iShaderID);
			glDeleteShader(m_iVertShader);
			glDeleteShader(m_iFragShader);

			return;
		}

		glDetachShader(m_iShaderID, m_iVertShader);
		glDetachShader(m_iShaderID, m_iFragShader);
	}

	void OpenGLShader::dispatchUnifoermUpload(ShaderDataType type, GLuint location, void* data)
	{
		const float* addrf;
		const int* addri;
		GLfloat valueFloat;
		GLint valueInt;

		switch (type)
		{
		case ShaderDataType::Int:
			valueInt = *(int*)data;
			glUniform1i(location, valueInt);
			break;
		case ShaderDataType::Int2:
			addri = (const int*)data;
			glUniform2iv(location, 1, addri);
			break;
		case ShaderDataType::Int3:
			addri = (const int*)data;
			glUniform3iv(location, 1, addri);
			break;
		case ShaderDataType::Int4:
			addri = (const int*)data;
			glUniform4iv(location, 1, addri);
			break;
		case ShaderDataType::Float:
			valueFloat = *(float*)data;
			glUniform1f(location, valueFloat);
			break;
		case ShaderDataType::Float2:
			addrf = (const float*)data;
			glUniform2fv(location, 1, addrf);
			break;
		case ShaderDataType::Float3:
			addrf = (const float*)data;
			glUniform3fv(location, 1, addrf);
			break;
		case ShaderDataType::Float4:
			addrf = (const float*)data;
			glUniform4fv(location, 1, addrf);
			break;
		case ShaderDataType::Mat3:
			addrf = (const float*)data;
			glUniformMatrix3fv(location, 1, GL_FALSE, addrf);
			break;
		case ShaderDataType::Mat4:
			addrf = (const float*)data;
			glUniformMatrix4fv(location, 1, GL_FALSE, addrf);
			break;
		case ShaderDataType::Bool:
			valueInt = *(bool*)data;
			glUniform1i(location, valueInt);
			break;
		case ShaderDataType::Sampler2D:
			valueInt = *(int*)data;
			glUniform1i(location, valueInt);
			break;
		}
	}
}