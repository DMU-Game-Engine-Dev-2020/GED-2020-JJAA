/** \file application.cpp
*/


#include "engine_pch.h"

#pragma region TempIncludes
// temp includes
#include <glad/glad.h>
#include <gl/GL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#pragma endregion TempIncludes

#include "core/application.h"

#ifdef NG_PLATFORM_WINDOWS
#include "platform/GLFW/GLFWWindowsSystem.h"
#endif // NG_PLATFORM_WINDOWS

namespace Engine 
{
	Application* Application::ms_instance = nullptr;


#pragma region TempGlobalVars
	glm::mat4 FCmodel, TPmodel;
#pragma endregion TempGlobalVars


	Application::Application()
	{
		if (ms_instance == nullptr)
		{
			ms_instance = this;
		}

		m_bRunning = true; // Application is running
		m_fTotalTimeElapsed = 0; // Set the time elapsed to nothing

		// Create and get instances of the objects
		m_pLogger = Log::getInstance();
		m_pLogger->start(SystemSignal::None);
		m_pTimer = Timer::getInstance();
		m_pTimer->start(SystemSignal::None);

#ifdef NG_PLATFORM_WINDOWS
		m_pWindows = GLFWWindowsSystem::getInstance();
#endif // NG_PLATFORM_WINDOWS
		m_pWindows->start(SystemSignal::None);
		LOG_INFO("Windows system initialised");

		// Create a window
		m_pWindow = std::unique_ptr<Window>(Window::create());
		// Set the windows event callback to call the onEvent function in Application
		m_pWindow->setEventCallback(std::bind(&Application::onEvent, this, std::placeholders::_1));

#pragma region TempSetup
		//  Temporary set up code to be abstracted
		
		// Enable standard depth detest (Z-buffer)
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		// Enabling backface culling to ensure triangle vertices are correct ordered (CCW)
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		m_pFCVertexArray.reset(VertexArray::create());

		float FCvertices[6 * 24] = {
			-0.5f, -0.5f, -0.5f, 0.8f, 0.2f, 0.2f, // red square
			 0.5f, -0.5f, -0.5f, 0.8f, 0.2f, 0.2f,
			 0.5f,  0.5f, -0.5f, 0.8f, 0.2f, 0.2f,
			-0.5f,  0.5f, -0.5f,  0.8f, 0.2f, 0.2f,
			-0.5f, -0.5f, 0.5f, 0.2f, 0.8f, 0.2f, // green square
			 0.5f, -0.5f, 0.5f, 0.2f, 0.8f, 0.2f,
			 0.5f,  0.5f, 0.5f, 0.2f, 0.8f, 0.2f,
			-0.5f,  0.5f, 0.5f,  0.2f, 0.8f, 0.2f,
			-0.5f, -0.5f, -0.5f, 0.8f, 0.2f, 0.8f, // magenta(ish) square
			 0.5f, -0.5f, -0.5f, 0.8f, 0.2f, 0.8f,
			 0.5f, -0.5f, 0.5f, 0.8f, 0.2f, 0.8f,
			-0.5f, -0.5f, 0.5f,  0.8f, 0.2f, 0.8f,
			-0.5f, 0.5f, -0.5f, 0.8f, 0.8f, 0.2f, // yellow square 
			 0.5f, 0.5f, -0.5f, 0.8f, 0.8f, 0.2f,
			 0.5f, 0.5f, 0.5f, 0.8f, 0.8f, 0.2f,
			-0.5f, 0.5f, 0.5f,  0.8f, 0.8f, 0.2f,
			-0.5f, -0.5f, -0.5f, 0.2f, 0.8f, 0.8f, // Cyan(ish) square 
			-0.5f,  0.5f, -0.5f,  0.2f, 0.8f, 0.8f,
			-0.5f,  0.5f, 0.5f, 0.2f, 0.8f, 0.8f,
			-0.5f,  -0.5f, 0.5f, 0.2f, 0.8f, 0.8f,
			0.5f, -0.5f, -0.5f, 0.2f, 0.2f, 0.8f, // Blue square 
			0.5f,  0.5f, -0.5f,  0.2f, 0.2f, 0.8f,
			0.5f,  0.5f, 0.5f, 0.2f, 0.2f, 0.8f,
			0.5f,  -0.5f, 0.5f, 0.2f, 0.2f, 0.8f
		};

		BufferLayout FCLayout =
		{
			{ ShaderDataType::Float3 },
			{ ShaderDataType::Float3 }
		};

		m_pFCVertexBuffer.reset(VertexBuffer::create(FCvertices, sizeof(FCvertices), FCLayout));
		m_pFCVertexArray->setVertexBuffer(m_pFCVertexBuffer);

		unsigned int indices[3 * 12] = {
			2, 1, 0,
			0, 3, 2,
			4, 5, 6,
			6, 7, 4,
			8, 9, 10,
			10, 11, 8,
			14, 13, 12,
			12, 15, 14,
			18, 17, 16,
			16, 19, 18,
			20, 21, 22,
			22, 23, 20
		};

		m_pFCIndexBuffer.reset(IndexBuffer::create(indices, sizeof(indices) / sizeof(unsigned int)));
		m_pFCVertexArray->setIndexBuffer(m_pFCIndexBuffer);

		std::string FCvertSrc = R"(
				#version 440 core
			
				layout(location = 0) in vec3 a_vertexPosition;
				layout(location = 1) in vec3 a_vertexColour;
				out vec3 fragmentColour;
				uniform mat4 u_MVP;
				void main()
				{
					fragmentColour = a_vertexColour;
					gl_Position =  u_MVP * vec4(a_vertexPosition,1);
				}
			)";

		std::string FCFragSrc = R"(
				#version 440 core
			
				layout(location = 0) out vec4 colour;
				in vec3 fragmentColour;
				void main()
				{
					colour = vec4(fragmentColour, 1.0);
				}
		)";

		GLuint FCVertShader = glCreateShader(GL_VERTEX_SHADER);

		const GLchar* source = FCvertSrc.c_str();
		glShaderSource(FCVertShader, 1, &source, 0);
		glCompileShader(FCVertShader);

		GLint isCompiled = 0;
		glGetShaderiv(FCVertShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(FCVertShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(FCVertShader, maxLength, &maxLength, &infoLog[0]);
			LOG_ERROR("Shader compile error: {0}", std::string(infoLog.begin(), infoLog.end()));

			glDeleteShader(FCVertShader);
			return;
		}

		GLuint FCFragShader = glCreateShader(GL_FRAGMENT_SHADER);

		source = FCFragSrc.c_str();
		glShaderSource(FCFragShader, 1, &source, 0);
		glCompileShader(FCFragShader);

		glGetShaderiv(FCFragShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(FCFragShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(FCFragShader, maxLength, &maxLength, &infoLog[0]);
			LOG_ERROR("Shader compile error: {0}", std::string(infoLog.begin(), infoLog.end()));

			glDeleteShader(FCFragShader);
			glDeleteShader(FCVertShader);

			return;
		}

		m_FCprogram = glCreateProgram();
		glAttachShader(m_FCprogram, FCVertShader);
		glAttachShader(m_FCprogram, FCFragShader);
		glLinkProgram(m_FCprogram);

		GLint isLinked = 0;
		glGetProgramiv(m_FCprogram, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_FCprogram, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_FCprogram, maxLength, &maxLength, &infoLog[0]);
			LOG_ERROR("Shader linking error: {0}", std::string(infoLog.begin(), infoLog.end()));

			glDeleteProgram(m_FCprogram);
			glDeleteShader(FCVertShader);
			glDeleteShader(FCFragShader);

			return;
		}

		glDetachShader(m_FCprogram, FCVertShader);
		glDetachShader(m_FCprogram, FCFragShader);

		/////////////////////////////////////////////////////////
		// Added textuer phong shader and cube //////////////////
		/////////////////////////////////////////////////////////

		m_pTPVertexArray.reset(VertexArray::create());

		float TPvertices[8 * 24] = {
			-0.5f, -0.5f, -0.5f, 0.f, 0.f, -1.f, 0.33f, 0.5f,
			 0.5f, -0.5f, -0.5f, 0.f, 0.f, -1.f, 0.f, 0.5f,
			 0.5f,  0.5f, -0.5f, 0.f, 0.f, -1.f, 0.f, 0.f,
			-0.5f,  0.5f, -0.5f, 0.f, 0.f, -1.f, 0.33f, 0.f,
			-0.5f, -0.5f, 0.5f,  0.f, 0.f, 1.f, 0.33f, 0.5f,
			 0.5f, -0.5f, 0.5f,  0.f, 0.f, 1.f, 0.66f, 0.5f,
			 0.5f,  0.5f, 0.5f,  0.f, 0.f, 1.f, 0.66f, 0.f,
			-0.5f,  0.5f, 0.5f,  0.f, 0.f, 1.f, 0.33, 0.f,
			-0.5f, -0.5f, -0.5f, 0.f, -1.f, 0.f, 1.f, 0.f,
			 0.5f, -0.5f, -0.5f, 0.f, -1.f, 0.f, 0.66f, 0.f,
			 0.5f, -0.5f, 0.5f,  0.f, -1.f, 0.f, 0.66f, 0.5f,
			-0.5f, -0.5f, 0.5f,  0.f, -1.f, 0.f, 1.0f, 0.5f,
			-0.5f, 0.5f, -0.5f,  0.f, 1.f, 0.f, 0.33f, 1.0f,
			 0.5f, 0.5f, -0.5f,  0.f, 1.f, 0.f, 0.f, 1.0f,
			 0.5f, 0.5f, 0.5f, 0.f, 1.f, 0.f, 0.f, 0.5f,
			-0.5f, 0.5f, 0.5f,   0.f, 1.f, 0.f, 0.3f, 0.5f,
			-0.5f, -0.5f, -0.5f, -1.f, 0.f, 0.f, 0.33f, 1.0f,
			-0.5f,  0.5f, -0.5f, -1.f, 0.f, 0.f, 0.33f, 0.5f,
			-0.5f,  0.5f, 0.5f,  -1.f, 0.f, 0.f, 0.66f, 0.5f,
			-0.5f,  -0.5f, 0.5f, -1.f, 0.f, 0.f, 0.66f, 1.0f,
			0.5f, -0.5f, -0.5f,  1.f, 0.f, 0.f, 1.0f, 1.0f,
			0.5f,  0.5f, -0.5f,  1.f, 0.f, 0.f, 1.0f, 0.5f,
			0.5f,  0.5f, 0.5f, 1.f, 0.f, 0.f,  0.66f, 0.5f,
			0.5f,  -0.5f, 0.5f,  1.f, 0.f, 0.f, 0.66f, 1.0f
		};

		BufferLayout TPLayout =
		{
			{ ShaderDataType::Float3 },
			{ ShaderDataType::Float3 },
			{ ShaderDataType::Float2 }
		};

		m_pTPVertexBuffer.reset(VertexBuffer::create(TPvertices, sizeof(TPvertices), TPLayout));
		m_pTPVertexArray->setVertexBuffer(m_pTPVertexBuffer);

		m_pTPIndexBuffer.reset(IndexBuffer::create(indices, sizeof(indices) / sizeof(unsigned int)));
		m_pTPVertexArray->setIndexBuffer(m_pTPIndexBuffer);

		std::string TPvertSrc = R"(
				#version 440 core
			
				layout(location = 0) in vec3 a_vertexPosition;
				layout(location = 1) in vec3 a_vertexNormal;
				layout(location = 2) in vec2 a_texCoord;
				out vec3 fragmentPos;
				out vec3 normal;
				out vec2 texCoord;
				uniform mat4 u_model;
				uniform mat4 u_MVP;
				void main()
				{
					fragmentPos = vec3(u_model * vec4(a_vertexPosition, 1.0));
					normal = mat3(transpose(inverse(u_model))) * a_vertexNormal;
					texCoord = vec2(a_texCoord.x, a_texCoord.y);
					gl_Position =  u_MVP * vec4(a_vertexPosition,1.0);
				}
			)";

		std::string TPFragSrc = R"(
				#version 440 core
			
				layout(location = 0) out vec4 colour;
				in vec3 normal;
				in vec3 fragmentPos;
				in vec2 texCoord;
				uniform vec3 u_lightPos; 
				uniform vec3 u_viewPos; 
				uniform vec3 u_lightColour;
				uniform sampler2D u_texData;
				void main()
				{
					float ambientStrength = 0.4;
					vec3 ambient = ambientStrength * u_lightColour;
					vec3 norm = normalize(normal);
					vec3 lightDir = normalize(u_lightPos - fragmentPos);
					float diff = max(dot(norm, lightDir), 0.0);
					vec3 diffuse = diff * u_lightColour;
					float specularStrength = 0.8;
					vec3 viewDir = normalize(u_viewPos - fragmentPos);
					vec3 reflectDir = reflect(-lightDir, norm);  
					float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
					vec3 specular = specularStrength * spec * u_lightColour;  
					
					colour = vec4((ambient + diffuse + specular), 1.0) * texture(u_texData, texCoord);
				}
		)";

		GLuint TPVertShader = glCreateShader(GL_VERTEX_SHADER);

		source = TPvertSrc.c_str();
		glShaderSource(TPVertShader, 1, &source, 0);
		glCompileShader(TPVertShader);

		isCompiled = 0;
		glGetShaderiv(TPVertShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(TPVertShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(TPVertShader, maxLength, &maxLength, &infoLog[0]);
			LOG_ERROR("Shader compile error: {0}", std::string(infoLog.begin(), infoLog.end()));

			glDeleteShader(TPVertShader);
			return;
		}

		GLuint TPFragShader = glCreateShader(GL_FRAGMENT_SHADER);

		source = TPFragSrc.c_str();
		glShaderSource(TPFragShader, 1, &source, 0);
		glCompileShader(TPFragShader);

		glGetShaderiv(TPFragShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(TPFragShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(TPFragShader, maxLength, &maxLength, &infoLog[0]);
			LOG_ERROR("Shader compile error: {0}", std::string(infoLog.begin(), infoLog.end()));

			glDeleteShader(TPFragShader);
			glDeleteShader(TPVertShader);

			return;
		}

		m_TPprogram = glCreateProgram();
		glAttachShader(m_TPprogram, TPVertShader);
		glAttachShader(m_TPprogram, TPFragShader);
		glLinkProgram(m_TPprogram);

		isLinked = 0;
		glGetProgramiv(m_TPprogram, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_TPprogram, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_TPprogram, maxLength, &maxLength, &infoLog[0]);
			LOG_ERROR("Shader linking error: {0}", std::string(infoLog.begin(), infoLog.end()));

			glDeleteProgram(m_TPprogram);
			glDeleteShader(TPVertShader);
			glDeleteShader(TPFragShader);

			return;
		}

		glDetachShader(m_TPprogram, FCVertShader);
		glDetachShader(m_TPprogram, FCFragShader);


		glGenTextures(1, &m_letterTexture);
		glActiveTexture(GL_TEXTURE0);
		m_textureSlots[0] = 0;
		glBindTexture(GL_TEXTURE_2D, m_letterTexture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, channels;

		unsigned char *data = stbi_load("assets/textures/letterCube.png", &width, &height, &channels, 0);
		if (data)
		{
			if (channels == 3) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			else if (channels == 4) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			else return;
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			return;
		}
		stbi_image_free(data);

		glGenTextures(1, &m_numberTexture);
		glActiveTexture(GL_TEXTURE0 + 1);
		m_textureSlots[1] = 1;
		glBindTexture(GL_TEXTURE_2D, m_numberTexture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		data = stbi_load("assets/textures/numberCube.png", &width, &height, &channels, 0);
		if (data)
		{
			if (channels == 3) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			else if (channels == 4) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			else return;
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			return;
		}
		stbi_image_free(data);

		FCmodel = glm::translate(glm::mat4(1), glm::vec3(1.5, 0, 3));
		TPmodel = glm::translate(glm::mat4(1), glm::vec3(-1.5, 0, 3));

		// End temporary code

#pragma endregion TempSetup


		TIMER_NEWFRAME; // Tell timer to start for a new frame
	}

	Application::~Application()
	{
		// Run the stop functions of the systems
		m_pWindows->stop(SystemSignal::None);
		m_pTimer->stop(SystemSignal::None);
		m_pLogger->stop(SystemSignal::None);

		// Set the pointers to the systems to null
		m_pWindows = nullptr;
		m_pTimer = nullptr;
		m_pLogger = nullptr;
	}

	void Application::onEvent(Event& e)
	{
		// Create event dispatcher
		EventDispatcher dispatcher(e);
		// If the Event type matches, call the corresponding function
		dispatcher.dispatch<WindowCloseEvent>(std::bind(&Application::onClose, this, std::placeholders::_1));
		dispatcher.dispatch<WindowResizeEvent>(std::bind(&Application::onResize, this, std::placeholders::_1));
		dispatcher.dispatch<WindowFocusEvent>(std::bind(&Application::onFocus, this, std::placeholders::_1));
		dispatcher.dispatch<WindowLostFocusEvent>(std::bind(&Application::onLostFocus, this, std::placeholders::_1));
		dispatcher.dispatch<WindowMovedEvent>(std::bind(&Application::onWindowMoved, this, std::placeholders::_1));
		dispatcher.dispatch<KeyPressedEvent>(std::bind(&Application::onKeyPressed, this, std::placeholders::_1));
	}

	bool Application::onClose(WindowCloseEvent& e)
	{
		// Log what's happening
		LOG_TRACE("Window closing");
		m_bRunning = false; // No longer running
		return true;
	}

	bool Application::onResize(WindowResizeEvent& e)
	{
		// Log what's happening
		LOG_TRACE("Window resize event. Width {0}, Height {1}", e.getWidth(), e.getHeight());
		m_pWindow->onResize(e.getWidth(), e.getHeight()); // Set the windows new size in its properties
		return true;
	}

	bool Application::onFocus(WindowFocusEvent& e)
	{
		// Log what's happening
		LOG_TRACE("Window being focused on");
		return true;
	}

	bool Application::onLostFocus(WindowLostFocusEvent& e)
	{
		// Log what's happening
		LOG_TRACE("Window losing focus");
		return true;
	}

	bool Application::onWindowMoved(WindowMovedEvent & e)
	{
		// Log what's happening
		LOG_TRACE("Window moved event. Position: {0}, {1}", e.getXPos(), e.getYPos());
		return true;
	}

	bool Application::onKeyPressed(KeyPressedEvent& e)
	{
		if (e.getKeyCode() == ENGINE_KEY_SPACE)
		{
			if (!m_pWindow->isFullScreenMode())
			{
				LOG_TRACE("Entering fullscreen");
				m_pWindow->setFullscreen(true);
			}
			else
			{
				LOG_TRACE("Exiting fullscreen");
				m_pWindow->setFullscreen(false);
			}
			return true;
		}
		if (e.getKeyCode() == ENGINE_KEY_ESCAPE)
		{
			LOG_TRACE("Window closing");
			m_bRunning = false;
			return true;
		}
		if (e.getKeyCode() == ENGINE_KEY_ENTER)
		{
			float mouseX = InputPoller::getMouseX();
			float mouseY = InputPoller::getMouseY();

			LOG_TRACE("Mouse at: {0}, {1}", mouseX, mouseY);
			return true;
		}
		return false;
	}

	void Application::run()
	{
		// While application is running
		while (m_bRunning)
		{
			TIMER_NEWFRAME; // Tell timer to start for a new frame

			m_fTimestep = TIMER_TIMESTEP; // Get the time taken to run the previous frame

			// Calculate and display the FPS
			float FPS = 1 / m_fTimestep;
			//LOG_INFO("FPS: {0}", FPS);

			m_fTotalTimeElapsed += m_fTimestep; // Add the time to run the previous frame to the total time elapsed


#pragma region TempDrawCode
// Temporary draw code to be abstracted

			glClearColor(0.8f, 0.8f, 0.8f, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f); // Basic 4:3 camera

			glm::mat4 view = glm::lookAt(
				glm::vec3(0.0f, 0.0f, -4.5f), // Camera is at (0.0,0.0,-4.5), in World Space
				glm::vec3(0.f, 0.f, 0.f), // and looks at the origin
				glm::vec3(0.f, 1.f, 0.f)  // Standing straight  up
			);

			// Code to make the cube move, you can ignore this more or less.
			glm::mat4 FCtranslation, TPtranslation;

			if (m_goingUp)
			{
				FCtranslation = glm::translate(FCmodel, glm::vec3(0.0f, 0.2f * m_fTimestep, 0.0f));
				TPtranslation = glm::translate(TPmodel, glm::vec3(0.0f, -0.2f * m_fTimestep, 0.0f));
			}
			else
			{
				FCtranslation = glm::translate(FCmodel, glm::vec3(0.0f, -0.2f * m_fTimestep, 0.0f));
				TPtranslation = glm::translate(TPmodel, glm::vec3(0.0f, 0.2f * m_fTimestep, 0.0f));
			}

			m_timeSummed += m_fTimestep;
			if (m_timeSummed > 20.0f) {
				m_timeSummed = 0.f;
				m_goingUp = !m_goingUp;
			}


			FCmodel = glm::rotate(FCtranslation, glm::radians(20.f) * m_fTimestep, glm::vec3(0.f, 1.f, 0.f)); // Spin the cube at 20 degrees per second
			TPmodel = glm::rotate(TPtranslation, glm::radians(-20.f) * m_fTimestep, glm::vec3(0.f, 1.f, 0.f)); // Spin the cube at 20 degrees per second

			// End of code to make the cube move.

			///////////////////////////////////////
			// colour cube ////////////////////////
			///////////////////////////////////////

			glm::mat4 fcMVP = projection * view * FCmodel;
			glUseProgram(m_FCprogram);
			m_pFCVertexArray->bind();
			GLuint MVPLoc = glGetUniformLocation(m_FCprogram, "u_MVP");
			glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, &fcMVP[0][0]);
			glDrawElements(GL_TRIANGLES, m_pFCIndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);

			///////////////////////////////////////
			// textured phong cube ////////////////
			///////////////////////////////////////

			glm::mat4 tpMVP = projection * view * TPmodel;
			unsigned int texSlot;
			if (m_goingUp) texSlot = m_textureSlots[0];
			else texSlot = m_textureSlots[1];

			glUseProgram(m_TPprogram);
			m_pTPVertexArray->bind();

			MVPLoc = glGetUniformLocation(m_TPprogram, "u_MVP");
			glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, &tpMVP[0][0]);

			GLuint modelLoc = glGetUniformLocation(m_TPprogram, "u_model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &TPmodel[0][0]);

			GLuint colLoc = glGetUniformLocation(m_TPprogram, "u_objectColour");
			glUniform3f(colLoc, 0.2f, 0.8f, 0.5f);

			GLuint lightColLoc = glGetUniformLocation(m_TPprogram, "u_lightColour");
			glUniform3f(lightColLoc, 1.0f, 1.0f, 1.0f);

			GLuint lightPosLoc = glGetUniformLocation(m_TPprogram, "u_lightPos");
			glUniform3f(lightPosLoc, 1.0f, 4.0f, -6.0f);

			GLuint viewPosLoc = glGetUniformLocation(m_TPprogram, "u_viewPos");
			glUniform3f(viewPosLoc, 0.0f, 0.0f, -4.5f);

			GLuint texDataLoc = glGetUniformLocation(m_TPprogram, "u_texData");
			glUniform1i(texDataLoc, texSlot);

			glDrawElements(GL_TRIANGLES, m_pTPIndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);

			// End temporary code
#pragma endregion TempDrawCode


			m_pWindow->onUpdate(m_fTimestep); // Update the window
		}

		m_pWindow->close();

		/*// Test the timer
		// Start two timers with the same tag
		TIMER_START("Tag");
		TIMER_START("Tag");

		// Get and display time from a timer with tag 'tag'
		float time = TIMER_END("Tag");
		LOG_INFO("Tag time:  {0}", time);

		// Try to end a timer with a tag that wasn't used to start a timer
		float time2 = TIMER_END("Test");
		LOG_INFO("Time:  {0}", time2);*/
	}
}
