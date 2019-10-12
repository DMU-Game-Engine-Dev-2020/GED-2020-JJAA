/** \file timer.h
*/
#pragma once

#include "system.h"
#include "chrono"
#include "log.h"

#include <map>
#include <string>

namespace Engine
{
	/**
	\class Timer
	A system class used for getting the time for a frame. Can also be used to find how long sections of code
	take to run
	*/

	class Timer : public System
	{
	private:
		static bool s_bInstanceFlag; //!< If the timer object exists
		static std::shared_ptr<Timer> s_pThis; //!< Pointer to Timer object

		static std::chrono::high_resolution_clock::time_point s_frameStart; //!< Time at the start of the frame
		static std::chrono::high_resolution_clock::time_point s_frameEnd; //!< Time at the end of the frame
		static std::chrono::duration<float> s_frameDuration; //!< Duration of the frame

		static std::map<std::string, std::chrono::high_resolution_clock::time_point> s_sectionTimeStart; //!< Map containing start times for timing sections of code and their corresponding tags
		static std::map<std::string, std::chrono::high_resolution_clock::time_point> s_sectionTimeEnd; //!< Map containing end times for timing sections of code and their corresponding tags
		static std::chrono::duration<float> s_sectionTimeDuration; //!< Used for calculating the time taken to run a section of code

		static bool s_bWorking; //!< Will be false if multiple timers with the same tag are started
	public:
		static std::shared_ptr<Timer> getInstance(); //!< Create instance of object and return a pointer to it

		void start(SystemSignal init, ...) override; //!< Start the timer
		void stop(SystemSignal close, ...) override; //!< Stop the timer

		static void newFrame(); //!< Calculate the time passed since the last frame
		inline static float timestep() { return s_frameDuration.count(); } //!< To return the current time between frames

		static void startTimer(std::string tag); //!< Start the timer for a section of code
		static float endTimer(std::string tag); //!< Stop the timer for the section of code
	};
}

// Macros for using the timer
#define TIMER_NEWFRAME()  Engine::Timer::newFrame()
#define TIMER_TIMESTEP()  Engine::Timer::timestep()
#define TIMER_START(...)  Engine::Timer::startTimer(__VA_ARGS__)
#define TIMER_END(...)    Engine::Timer::endTimer(__VA_ARGS__)