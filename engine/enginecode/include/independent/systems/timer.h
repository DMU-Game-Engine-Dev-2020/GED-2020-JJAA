/** \file timer.h
*/
#pragma once

#include "system.h"
#include "chrono"
#include "log.h"

#include <map>

namespace Engine
{
	/**
	\class Timer
	\brief A system class used for timing things
	
	It is used to get the time taken for a frame. It can also be used to find how long sections of code take to run
	*/
	class Timer : public System
	{
	private:
		static bool s_bInstanceFlag; //!< If the timer object exists
		static std::shared_ptr<Timer> s_pThis; //!< Pointer to Timer object

		static std::chrono::high_resolution_clock::time_point s_frameStart; //!< Time at the start of the frame
		static std::chrono::high_resolution_clock::time_point s_frameEnd; //!< Time at the end of the frame
		static std::chrono::duration<float> s_frameDuration; //!< Duration of the frame

		static std::map<std::string, std::pair<std::chrono::high_resolution_clock::time_point, std::chrono::high_resolution_clock::time_point>> s_sectionTimer; //!< Map containing time points and corresponding tag for timing sections of code
		static std::chrono::duration<float> s_sectionTimeDuration; //!< Used for calculating the time taken to run a section of code

		static bool s_bWorking; //!< Will be false if multiple timers with the same tag are started
	public:
		//! Create instance of object and return a pointer to it
		/*!
		\return A pointer to the instance of the object
		*/
		static std::shared_ptr<Timer> getInstance();

		void start(SystemSignal init = SystemSignal::None, ...) override; //!< Start the timer
		void stop(SystemSignal close = SystemSignal::None, ...) override; //!< Stop the timer

		static float newFrame(); //!< Calculate the time passed since the last frame

		//! Used to return the timestep
		/*!
		\return The time taken to render the previous frame
		*/
		inline static float timestep() { return s_frameDuration.count(); } 


		//! Start the timer for a section of code
		/*!
		\param tag The tag used to access the correct time points in the map
		*/
		static void startTimer(std::string tag); 
		//! Stop the timer for the section of code
		/*!
		\param tag The tag used to access the correct time points in the map
		\return The time passed since starting the timer
		*/
		static float endTimer(std::string tag); 
	};
}

//! Macro to start a new timer for a new frame
#define TIMER_NEWFRAME    Engine::Timer::newFrame()

//! Macro to get the timestep
#define TIMER_TIMESTEP    Engine::Timer::timestep()

//! Macro to stop a timer and get the time
#define TIMER_END(...)    Engine::Timer::endTimer(__VA_ARGS__)

//! Macro to start a new timer
#define TIMER_START(...)  Engine::Timer::startTimer(__VA_ARGS__)

