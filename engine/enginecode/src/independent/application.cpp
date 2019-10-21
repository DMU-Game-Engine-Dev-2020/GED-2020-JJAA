/** \file application.cpp
*/


#include "engine_pch.h"
#include "core/application.h"

namespace Engine {
	Application* Application::ms_instance = nullptr;

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
		m_pTimer = Timer::getInstance();

		// Run the start functions of the systems
		m_pLogger->start(SystemSignal::None);
		m_pTimer->start(SystemSignal::None);

		// Create a window
		m_pWindow = std::unique_ptr<Window>(Window::create());
		// Set the windows event callback to call the onEvent function in Application
		m_pWindow->setEventCallback(std::bind(&Application::onEvent, this, std::placeholders::_1));
	}

	Application::~Application()
	{
		// Run the stop functions of the systems
		m_pTimer->stop(SystemSignal::None);
		m_pLogger->stop(SystemSignal::None);

		// Set the pointers to the systems to null
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

	bool Application::onKeyPressed(KeyPressedEvent& e)
	{
		if (e.getCeyCode() == ENGINE_KEY_SPACE)
		{
			if (!m_pWindow->isFullScreenMode())
			{
				LOG_TRACE("Entering fullscreen");
				m_pWindow->setFullscreen(true);
			}
			else if (m_pWindow->isFullScreenMode())
			{
				LOG_TRACE("Exiting fullscreen");
				m_pWindow->setFullscreen(false);
			}
		}
		return true;
	}

	void Application::run()
	{
		// While application is running
		while (m_bRunning)
		{
			// Log information
			//LOG_INFO("Frame, FRAME, FRAAAAME!");
			TIMER_NEWFRAME; // Tell timer to start for new frame

			m_fLastFrameTime = TIMER_TIMESTEP; // Get the time taken to run the previous frame

			// Calculate and display the FPS
			float FPS = 1 / m_fLastFrameTime;
			//LOG_INFO("FPS: {0}", FPS);

			m_fTotalTimeElapsed += m_fLastFrameTime; // Add the time to run the previous frame to the total time elapsed

			m_pWindow->onUpdate(m_fLastFrameTime); // Update the window
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
