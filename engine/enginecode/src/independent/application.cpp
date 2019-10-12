/** \file application.cpp
*/


#include "engine_pch.h"
#include "core/application.h"


namespace Engine {
	Application* Application::s_instance = nullptr;

	Application::Application()
	{
		if (s_instance == nullptr)
		{
			s_instance = this;
		}

		bRunning = true; // Application is running
		fTotalTimeElapsed = 0; // Set the time elapsed to nothing

		// Create and get instances of the objects
		pLog = Log::getInstance();
		pTimer = Timer::getInstance();

		// Run the start functions of the systems
		pLog->start(SystemSignal::None);
		pTimer->start(SystemSignal::None);
	}

	Application::~Application()
	{
		// Run the stop functions of the systems
		pTimer->stop(SystemSignal::None);
		pLog->stop(SystemSignal::None);

		// Set the pointers to the systems to null
		pTimer = nullptr;
		pLog = nullptr;
	}

	

	void Application::run()
	{
		// While application is running
		while (bRunning)
		{
			// Log information
			LOG_INFO("Frame, FRAME, FRAAAAME!");
			TIMER_NEWFRAME(); // Tell timer to start for new frame

			fLastFrameTime = TIMER_TIMESTEP(); // Get the time taken to run the previous frame

			// Calculate and display the FPS
			float FPS = 1 / fLastFrameTime;
			LOG_INFO("FPS: {0}", FPS);

			fTotalTimeElapsed += fLastFrameTime; // Add the time to run the previous frame to the total time elapsed
			// If 5 seconds have passed
			if (fTotalTimeElapsed >= 5.f)
				bRunning = false; // No longer running
		}
		// Show the total time the application has been running
		LOG_INFO("Application running loop ended after {0} seconds", fTotalTimeElapsed);

		// Test the timer
		// Start two timers with the same tag
		TIMER_START("Tag");
		TIMER_START("Tag");

		// Get and display time from a timer with tag 'tag'
		float time = TIMER_END("Tag");
		LOG_INFO("Tag time:  {0}", time);
		
		// Try to end a timer with a tag that wasn't used to start a timer
		float time2 = TIMER_END("Test");
		LOG_INFO("Time:  {0}", time2);
	}
}
