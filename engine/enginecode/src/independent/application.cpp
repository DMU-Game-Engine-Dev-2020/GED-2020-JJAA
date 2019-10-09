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

		bRunning = true;
		fTotalTimeElapsed = 0;

		Log::start();
		
		pTimer = Timer::getInstance();
		pTimer->start(SystemSignal::None);
	}

	Application::~Application()
	{
		pTimer->stop(SystemSignal::None);
		Log::stop();
	}

	

	void Application::run()
	{
		while (bRunning)
		{
			LOG_INFO("Frame, FRAME, FRAAAAME!");

			fLastFrameTime = pTimer->timestep();

			float FPS = 1 / fLastFrameTime;
			LOG_INFO("FPS: {0}", FPS);

			fTotalTimeElapsed += fLastFrameTime;
			if (fTotalTimeElapsed >= 5.f)
				bRunning = false;
		}

		LOG_INFO("Application run loop ended after {0} seconds", fTotalTimeElapsed);
	}

}
