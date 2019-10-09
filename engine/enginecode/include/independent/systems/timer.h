#pragma once

#include "system.h"
#include "chrono"

namespace Engine
{
	class Timer : public System
	{
	private:
		static bool s_bRunning;
		static std::shared_ptr<Timer> s_pThis;

		std::chrono::high_resolution_clock::time_point s_frameStart;
		std::chrono::high_resolution_clock::time_point s_frameEnd;
		std::chrono::duration<float> s_frameDuration;
	public:
		static std::shared_ptr<Timer> getInstance();

		void start(SystemSignal init, ...);
		void stop(SystemSignal close, ...);

		float timestep();
	};
}