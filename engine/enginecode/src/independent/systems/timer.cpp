#include "engine_pch.h"
#include "systems/timer.h"

namespace Engine
{
	bool Timer::s_bRunning = false;
	std::shared_ptr<Timer> Timer::s_pThis = nullptr;

	std::shared_ptr<Timer> Timer::getInstance()
	{
		if (!s_bRunning)
		{
			s_pThis = std::shared_ptr<Timer>(new Timer());
			s_bRunning = true;

			return s_pThis;
		}
		else
			return s_pThis;
	}

	void Timer::start(SystemSignal init, ...)
	{
		s_frameStart = std::chrono::high_resolution_clock::now();
		s_bRunning = true;
	}

	void Timer::stop(SystemSignal close, ...)
	{
		s_bRunning = false;
		s_pThis = nullptr;
	}

	float Timer::timestep()
	{
		s_frameEnd = std::chrono::high_resolution_clock::now();
		s_frameDuration = s_frameEnd - s_frameStart;
		s_frameStart = std::chrono::high_resolution_clock::now();
		return s_frameDuration.count();
	}
}