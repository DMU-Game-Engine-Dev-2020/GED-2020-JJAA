/** \file application.cpp
*/
#include "engine_pch.h"

#include "core/application.h"
#include "core/core.h"

#ifdef NG_PLATFORM_WINDOWS
#include "platform/GLFW/GLFWWindowsSystem.h"
#endif // NG_PLATFORM_WINDOWS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Engine 
{
	Application* Application::s_instance = nullptr;


#pragma region TempGlobalVars
	glm::mat4 FCmodel, TPmodel;
#pragma endregion TempGlobalVars


	Application::Application()
	{
		if (s_instance == nullptr)
		{
			s_instance = this;
		}

		m_bRunning = true; // Application is running
		m_fTotalTimeElapsed = 0; // Set the time elapsed to nothing

		// Create and get instances of the system objects
		m_pLogger = Log::getInstance();
		m_pLogger->start();
		m_pTimer = Timer::getInstance();
		m_pTimer->start();

#ifdef NG_PLATFORM_WINDOWS // If the engine is running on a windows computer
		m_pWindows = GLFWWindowsSystem::getInstance(); // Create an instance of the GLFW windows system
#endif // NG_PLATFORM_WINDOWS
		m_pWindows->start(); // Start the windows system
		LOG_INFO("Windows system initialised");

		// Create a window
		m_pWindow = std::unique_ptr<Window>(Window::create());
		// Set the windows event callback to call the onEvent function in Application
		m_pWindow->setEventCallback(std::bind(&Application::onEvent, this, std::placeholders::_1));

		m_pResources.reset(new ResourceManager);

		m_pRenderer = std::shared_ptr<Renderer>(Renderer::createSimple3D());


		m_pRenderer->actionCommand(RenderCommand::setDepthTestLessCommand(true));
		m_pRenderer->actionCommand(RenderCommand::setBackfaceCullingCommand(true));

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
		
		m_pFCShader = m_pResources->addShader("FCShader", "assets/shaders/flatColourVert.glsl", "assets/shaders/flatColourFrag.glsl");
		m_pFCVAO = m_pResources->addVAO("flatColourCube");
		m_pFCVAO->setVertexBuffer(m_pResources->addVBO("flatColourVBO", FCvertices, sizeof(FCvertices), m_pFCShader->getBufferLayout()));
		m_pFCVAO->setIndexBuffer(m_pResources->addIndexBuffer("cubeIndices", indices, sizeof(indices) / sizeof(unsigned int)));

		m_pFCMat.reset(Material::create(m_pFCShader, m_pFCVAO));

		/////////////////////////////////////////////////////////
		// Added textuer phong shader and cube //////////////////
		/////////////////////////////////////////////////////////

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

		m_pTPShader = m_pResources->addShader("assets/shaders/texturedPhong.glsl");
		m_pTPVAO = m_pResources->addVAO("texturedPhongCube");
		m_pTPVAO->setVertexBuffer(m_pResources->addVBO("texturedPhongVBO", TPvertices, sizeof(TPvertices), m_pTPShader->getBufferLayout()));
		m_pTPVAO->setIndexBuffer(m_pResources->addIndexBuffer("cubheIndices", indices, sizeof(indices) / sizeof(unsigned int)));

		m_pLetterCubeTexture = m_pResources->addTexture("assets/textures/letterCube.png");
		m_pNumberCubeTexture = m_pResources->addTexture("assets/textures/numberCube.png");

		m_pTPMat.reset(Material::create(m_pTPShader, m_pTPVAO));

		FCmodel = glm::translate(glm::mat4(1), glm::vec3(1.5, 0, 3));
		TPmodel = glm::translate(glm::mat4(1), glm::vec3(-1.5, 0, 3));


		TIMER_NEWFRAME; // Tell the timer to start for a new frame
	}

	Application::~Application()
	{
		// Run the stop functions of the systems
		m_pResources->stop();
		m_pWindows->stop();
		m_pTimer->stop();
		m_pLogger->stop();
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
		// If the space key is pressed
		if (e.getKeyCode() == ENGINE_KEY_SPACE)
		{
			// If the window is not in fullscreen mode
			if (!m_pWindow->isFullScreenMode())
			{
				LOG_TRACE("Entering fullscreen"); // Log what's happening
				m_pWindow->setFullscreen(true); // Set the window to fullscreen
			}
			else // If the window is in fullscreen mode
			{
				LOG_TRACE("Exiting fullscreen"); // Log what's happening
				m_pWindow->setFullscreen(false); // Set the window to not fullscreen
			}
			return true; // Leave the function
		}
		// If escape key is pressed
		if (e.getKeyCode() == ENGINE_KEY_ESCAPE)
		{
			LOG_TRACE("Window closing"); // Log what's happening
			m_bRunning = false; // Stop the application from running
			return true; // Leave the function
		}
		// If the enter key is pressed
		if (e.getKeyCode() == ENGINE_KEY_ENTER)
		{
			// Use the input poller to get the mouse position
			float mouseX = InputPoller::getMouseX();
			float mouseY = InputPoller::getMouseY();

			LOG_TRACE("Mouse at: {0}, {1}", mouseX, mouseY); // Log the mouse position
			return true; // Leave the function
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


			m_pRenderer->actionCommand(RenderCommand::setClearColourCommand(0.8f, 0.8f, 0.8f, 1));
			m_pRenderer->actionCommand(RenderCommand::clearDepthColourBufferCommand());

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

			m_pFCMat->setDataElement("u_MVP", (void*)&fcMVP[0][0]);
			m_pRenderer->submit(m_pFCMat);

			///////////////////////////////////////
			// textured phong cube ////////////////
			///////////////////////////////////////

			glm::mat4 tpMVP = projection * view * TPmodel;
			unsigned int texSlot;
			if (m_goingUp) texSlot = m_pLetterCubeTexture->getSlot();
			else texSlot = m_pNumberCubeTexture->getSlot();

			glm::vec3 lightColour = glm::vec3(1.0f, 1.0f, 1.0f);
			glm::vec3 lightPos = glm::vec3(1.0f, 4.0f, -6.0f);
			glm::vec3 viewPos = glm::vec3(0.0f, 0.0f, -4.5f);
			
			m_pTPMat->setDataElement("u_MVP", (void*)&tpMVP[0][0]);
			m_pTPMat->setDataElement("u_model", (void*)&TPmodel[0][0]);
			   
			m_pTPMat->setDataElement("u_lightColour", (void*)&lightColour[0]);
			m_pTPMat->setDataElement("u_lightPos", (void*)&lightPos[0]);
			m_pTPMat->setDataElement("u_viewPos", (void*)&viewPos[0]);
			m_pTPMat->setDataElement("u_texData", (void*)&texSlot);

			m_pRenderer->submit(m_pTPMat);

			
			m_pWindow->onUpdate(m_fTimestep); // Update the window
		}

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
