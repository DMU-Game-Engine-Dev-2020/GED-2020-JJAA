/** \file OpenGLShader.cpp
*/
#include "engine_pch.h"

#include "platform/openGL/OpenGLShader.h"
#include "platform/openGL/OpenGLUniformObject.h"
#include "systems/log.h"
#include "systems/timer.h"

#include <glm/glm.hpp>
#include <fstream>
#include <sstream>

namespace Engine
{
	//! Used to get the ShaderDataType from the shader file string
	/*!
	\param glslSrc The string being checked
	\return The shader data type
	*/
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
		// Create the shaders
		m_iVertShader = glCreateShader(GL_VERTEX_SHADER);
		m_iFragShader = glCreateShader(GL_FRAGMENT_SHADER);

		parseSource(filepath); // Parse the file

		storeUniformLocations(); // Store the uniform locations
	}

	OpenGLShader::OpenGLShader(const std::string& vertexFilepath, const std::string& fragmentFilepath)
	{
		// Create the shaders
		m_iVertShader = glCreateShader(GL_VERTEX_SHADER);
		m_iFragShader = glCreateShader(GL_FRAGMENT_SHADER);

		parseSource(vertexFilepath, fragmentFilepath); // Parse the file

		storeUniformLocations(); // Store the uniform locations
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_iShaderID); // Free memory
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
		UniformLayout::iterator it; // Make an iterator for the uniform layout

		it = m_uniformLayout.find(name); // Find the name in the map

		// If the name is not found
		if (it == m_uniformLayout.end())
		{
			// Log an error
			LOG_ERROR("No uniform variable with name: {0} found in shaders", name);
		}
		else // If the name is found
		{
			// Upload the uniform with the data from the uniform layout map
			dispatchUnifoermUpload(it->second.first, (GLuint)it->second.second, data);
		}

		//Maybe
		//m_uniformLayout2.find(name)->second->uniformUpload(data);

		return true;
	}

	bool OpenGLShader::uploadData(const UniformLayout& uniforms)
	{
		m_uniformLayout = uniforms; // Set the uniform layout to the one passed in
		return true;
	}

	void OpenGLShader::parseSource(const std::string& filepath)
	{
		std::fstream handle(filepath, std::ios::in);
		enum { NONE = -1, VERTEX = 0, FRAGMENT } region; // For the region in the shader file

		// If the file is not open
		if (!handle.is_open()) 
			LOG_CRITICAL("Could not open shader file '{0}'", filepath); // Log a critical error

		std::string line; // For the line being got
		std::string src[2]; // One for vertex and one for fragment shader

		// Get a line from the file, while there is a line to be got
		while (getline(handle, line))
		{
			// If in the vertex region set the region and skip to the start of the loop again
			if (line.find("#region Vertex") != std::string::npos) { region = VERTEX; continue; }
			// If in the fragment region set the region and skip to the start of the loop again
			if (line.find("#region Fragment") != std::string::npos) { region = FRAGMENT; continue; }
			// If in the geometry region do nothing
			if (line.find("#region Geometry") != std::string::npos) { region = NONE; continue; }
			// If in the tessalation region do nothing
			if (line.find("#region Tessalation") != std::string::npos) { region = NONE; continue; }

			// If in the vertex region and 'in ' is in the line
			if (region == VERTEX && line.find("in ") != std::string::npos)
			{
				extractBufferLayout(line); // Get the buffer layout
			}

			// If in a region
			if (region != NONE)
			{
				// If 'uniform' is in the line
				if (line.find("uniform") != std::string::npos)
				{
					identifyUniform(line); // Identify the uniform
				}
				// Add the line to the region string
				src[region] += (line + "\n");
			}
		}
		
		handle.close(); // Close the file

		compileAndLink(src[VERTEX], src[FRAGMENT]); // Compile and link the shader
	}

	void OpenGLShader::parseSource(const std::string& vertexFilepath, const std::string& fragmentFilepath)
	{
		// Parsing the vertex shader file
		std::fstream vertexHandle(vertexFilepath, std::ios::in);
		enum { NONE = -1, VERTEX = 0, FRAGMENT }; // For the shader file being read

		// If the file is not open
		if (!vertexHandle.is_open())
			LOG_CRITICAL("Could not open shader file '{0}'", vertexFilepath); // Log a critical error

		std::string line; // For the line being got
		std::string src[2]; // One for vertex and one for fragment shader

		// Get a line from the file, while there is a line to be got
		while (getline(vertexHandle, line))
		{
			// If 'in ' is in the line
			if (line.find("in ") != std::string::npos)
			{
				extractBufferLayout(line); // Get the buffer layout
			}
			// If 'uniform' is in the line
			if (line.find("uniform") != std::string::npos)
			{
				identifyUniform(line); // Identify the uniform
			}
			// Add the line to the vertex string
			src[VERTEX] += (line + "\n");
		}
		// Close the file
		vertexHandle.close();

		// Parsing the fragment shader file
		std::fstream fragmentHandle(fragmentFilepath, std::ios::in);

		// If the file is not open
		if (!fragmentHandle.is_open())
			LOG_CRITICAL("Could not open shader file '{0}'", fragmentFilepath); // Log a critical error

		// Get a line from the file, while there is a line to be got
		while (getline(fragmentHandle, line))
		{
			// If 'uniform' is in the line
			if (line.find("uniform") != std::string::npos)
			{
				identifyUniform(line); // Identify the uniform
			}
			// Add the line to the fragment string
			src[FRAGMENT] += (line + "\n");
		}
		// Close the file
		fragmentHandle.close();

		compileAndLink(src[VERTEX], src[FRAGMENT]); // Compile and link the shader
	}

	void OpenGLShader::extractBufferLayout(const std::string& line)
	{
		std::string type; // The type

		std::stringstream iss(line);
		bool isType = false;

		while (iss >> type)
		{
			// Get each word on the line, if the current word is 'in', set type to the next word and leave the loop
			if (!isType)
			{
				if (type == "in")
					isType = true;
			}
			else
			{
				break;
			}
		}
		// Add the element to the buffer layout as a ShaderDataType
		m_bufferLayout.addElement(GLSLStrToSDT(type));
	}

	void OpenGLShader::identifyUniform(const std::string& line)
	{
		std::string type; // The type
		std::string name; // The name

		std::stringstream iss(line);
		bool isType = false;

		while (iss >> type)
		{
			// Get each word from the line, if the current word is 'uniform', 
			// set the next word to type and the one after to name
			if (!isType)
			{
				if (type == "uniform")
					isType = true;
			}
			else
			{
				iss >> name;
				name.erase(std::remove(name.begin(), name.end(), ';'), name.end());
				break;
			}
		}
		// Add the name, type as a ShaderDataType, and 0 for the location (can't be found yet) to the uniform layout
		m_uniformLayout.insert(std::make_pair(name, std::make_pair(GLSLStrToSDT(type), 0)));

		//Maybe
		//m_uniformLayout2.insert(std::make_pair(name, new OpenGLUniformObject(name, GLSLStrToSDT(type))));
	}

	void OpenGLShader::storeUniformLocations()
	{
		// For each thing in the uniform layout map
		for (const auto& it : m_uniformLayout)
		{
			// Get the uniform location
			GLuint location = glGetUniformLocation(m_iShaderID, it.first.c_str());
			// Set the location to the one found
			m_uniformLayout.find(it.first)->second.second = (int)location;
		}

		//Maybe
		/*for (auto& it : m_uniformLayout2)
		{
			switch (it.second->getType())
			{
			case ShaderDataType::Int:
				it.second->setLocationAndFunction(m_iShaderID,
					[](GLuint location, void* data)
				{
					GLint valueInt;
					valueInt = *(int*)data;
					glUniform1i(location, valueInt);
					return true;
				});
				break;
			case ShaderDataType::Int2:
				it.second->setLocationAndFunction(m_iShaderID,
					[](GLuint location, void* data)
				{
					const int* addri;
					addri = (const int*)data;
					glUniform2iv(location, 1, addri);
					return true;
				});
				break;
			case ShaderDataType::Int3:
				it.second->setLocationAndFunction(m_iShaderID,
					[](GLuint location, void* data)
				{
					const int* addri;
					addri = (const int*)data;
					glUniform3iv(location, 1, addri);
					return true;
				});
				break;
			case ShaderDataType::Int4:
				it.second->setLocationAndFunction(m_iShaderID,
					[](GLuint location, void* data)
				{
					const int* addri;
					addri = (const int*)data;
					glUniform4iv(location, 1, addri);
					return true;
				});
				break;
			case ShaderDataType::Float:
				it.second->setLocationAndFunction(m_iShaderID,
					[](GLuint location, void* data)
				{
					GLfloat valueFloat;
					valueFloat = *(float*)data;
					glUniform1f(location, valueFloat);
					return true;
				});
				break;
			case ShaderDataType::Float2:
				it.second->setLocationAndFunction(m_iShaderID,
					[](GLuint location, void* data)
				{
					const float* addrf;
					addrf = (const float*)data;
					glUniform2fv(location, 1, addrf);
					return true;
				});
				break;
			case ShaderDataType::Float3:
				it.second->setLocationAndFunction(m_iShaderID,
					[](GLuint location, void* data)
				{
					const float* addrf;
					addrf = (const float*)data;
					glUniform3fv(location, 1, addrf);
					return true;
				});
				break;
			case ShaderDataType::Float4:
				it.second->setLocationAndFunction(m_iShaderID,
					[](GLuint location, void* data)
				{
					const float* addrf;
					addrf = (const float*)data;
					glUniform4fv(location, 1, addrf);
					return true;
				});
				break;
			case ShaderDataType::Mat3:
				it.second->setLocationAndFunction(m_iShaderID,
					[](GLuint location, void* data)
				{
					const float* addrf;
					addrf = (const float*)data;
					glUniformMatrix3fv(location, 1, GL_FALSE, addrf);
					return true;
				});
				break;
			case ShaderDataType::Mat4:
				it.second->setLocationAndFunction(m_iShaderID,
					[](GLuint location, void* data)
				{
					const float* addrf;
					addrf = (const float*)data;
					glUniformMatrix4fv(location, 1, GL_FALSE, addrf);
					return true;
				});
				break;
			case ShaderDataType::Bool:
				it.second->setLocationAndFunction(m_iShaderID,
					[](GLuint location, void* data)
				{
					GLint valueInt;
					valueInt = *(bool*)data;
					glUniform1i(location, valueInt);
					return true;
				});
				break;
			case ShaderDataType::Sampler2D:
				it.second->setLocationAndFunction(m_iShaderID,
					[](GLuint location, void* data)
				{
					GLint valueInt;
					valueInt = *(int*)data;
					glUniform1i(location, valueInt);
					return true;
				});
				break;
			}
		}*/
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