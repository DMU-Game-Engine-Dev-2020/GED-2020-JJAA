/** \file application.cpp
*/
#include "engine_pch.h"

#include "core/application.h"
#include "core/core.h"
#include "core/codes.h"

#ifdef NG_PLATFORM_WINDOWS
#include "platform/GLFW/GLFWWindowsSystem.h"
#endif // NG_PLATFORM_WINDOWS

namespace Engine 
{
	Application* Application::s_instance = nullptr;

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
		m_pWindows.reset(new GLFWWindowsSystem); // Create an instance of the GLFW windows system
#endif // NG_PLATFORM_WINDOWS
		m_pWindows->start(); // Start the windows system
		LOG_INFO("Windows system initialised");

		// Create a window
		m_pWindow.reset(Window::create());
		// Set the windows event callback to call the onEvent function in Application
		m_pWindow->setEventCallback(std::bind(&Application::onEvent, this, std::placeholders::_1));

		// Make a new resource manager
		m_pResources.reset(new ResourceManager);

		m_pLayerStack.reset(new Layerstack);
		m_pLayerStack->start();

		TIMER_NEWFRAME; // Tell the timer to start for a new frame
	}

	Application::~Application()
	{
		// Run the stop functions of the systems
		m_pLayerStack->stop();
		m_pResources->stop();
		m_pWindow->close(); // Close the window
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
		dispatcher.dispatch<KeyPressedEvent>(std::bind(&Application::onKeyPressed, this, std::placeholders::_1));

		// Reverse iterate through the layer stack, once an event has been handled, leave the loop
		for (auto it = m_pLayerStack->rbegin(); it != m_pLayerStack->rend(); ++it)
		{
			(*it)->onEvent(e);
			if (e.handled()) break;
		}
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
		return false;
	}

	void Application::run()
	{
		while (m_bRunning)
		{
			m_fTimestep = TIMER_NEWFRAME; // Tell timer to start for a new frame
		
#ifdef NG_DEBUG
			LOG_INFO("FPS: {0} T: {1}", (int)(1 / m_fTimestep), m_fTimestep); // Show the FPS
#endif
			// Update all layers in the layer stack
			for (auto it = m_pLayerStack->begin(); it != m_pLayerStack->end(); it++)
				(*it)->onUpdate(m_fTimestep);
			// Update the window
			m_pWindow->onUpdate(m_fTimestep);
		}
	}
}
