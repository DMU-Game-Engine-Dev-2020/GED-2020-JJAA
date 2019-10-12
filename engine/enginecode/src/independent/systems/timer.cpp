/** \file timer.cpp
*/

#include "engine_pch.h"
#include "systems/timer.h"

namespace Engine
{
	bool Timer::s_bInstanceFlag = false; // Initialise instance flag as false
	std::shared_ptr<Timer> Timer::s_pThis = nullptr; // Initialise pointer to this object as null

	// Initialise all static time points and durations
	std::chrono::high_resolution_clock::time_point Timer::s_frameStart = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock::time_point Timer::s_frameEnd = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> Timer::s_frameDuration = s_frameEnd - s_frameStart;

	std::map<std::string, std::chrono::high_resolution_clock::time_point> Timer::s_sectionTimeStart;
	std::map<std::string, std::chrono::high_resolution_clock::time_point> Timer::s_sectionTimeEnd;
	std::chrono::duration<float> Timer::s_sectionTimeDuration = s_frameEnd - s_frameStart;

	bool Timer::s_bWorking = true; // Initialise the timer as working

	std::shared_ptr<Timer> Timer::getInstance()
	{
		// If there is no instance of the object
		if (!s_bInstanceFlag)
		{
			// Create a new object
			s_pThis = std::shared_ptr<Timer>(new Timer());
			s_bInstanceFlag = true; // There is an instance of the object

			return s_pThis; // Return the pointer to the object
		}
		else // If the object already exists
			return s_pThis; // Return the pointer to the object
	}

	void Timer::start(SystemSignal init, ...)
	{
		// Get the time now for calculating the time for a frame
		s_frameStart = std::chrono::high_resolution_clock::now();
	}

	void Timer::stop(SystemSignal close, ...)
	{
		// No longer an instance of the object
		s_bInstanceFlag = false;
		s_pThis = nullptr; // Set the pointer to itself to null
	}

	void Timer::newFrame()
	{
		s_frameEnd = std::chrono::high_resolution_clock::now(); // Get the time for the end of the frame
		s_frameDuration = s_frameEnd - s_frameStart; // Calculate the time taken for the frame
		s_frameStart = std::chrono::high_resolution_clock::now(); // Get the time for the start of the next frame
	}

	void Timer::startTimer(std::string tag)
	{
		// If the map doesn't contain the new tag
		if (s_sectionTimeStart.find(tag) == s_sectionTimeStart.end()) 
		{
			// Add time points to the start and end time maps with the new tag
			s_sectionTimeEnd.insert(std::make_pair(tag, std::chrono::high_resolution_clock::now()));
			s_sectionTimeStart.insert(std::make_pair(tag, std::chrono::high_resolution_clock::now()));
		}
		else // If the map already contains the tag
		{
			// Display error using the logger
			LOG_ERROR("Timer with tag: '{0}' already exists, cannot have two timers with the same tag", tag);
			s_bWorking = false; // Used to inform the user that timer outputs may be incorrect because multiple timers with the same tag were started
		}
	}

	float Timer::endTimer(std::string tag)
	{
		// If the map doesn't contain the tag
		if (s_sectionTimeStart.find(tag) == s_sectionTimeStart.end())
		{
			// Display error using the logger
			LOG_ERROR("Timer with tag: '{0}' not found, please ensure the same tag is used in start and end timer functions", tag);
			return 0.f; // Return nothing
		}
		else // If the map contains the tag
		{
			// If there has been an error
			if (!s_bWorking)
				LOG_WARN("Two timers with the same tag were started, output may be inaccurate"); // Warn the user

			// Set the end time point for the timer
			s_sectionTimeEnd[tag] = std::chrono::high_resolution_clock::now();
			// Calculate the timer duration
			s_sectionTimeDuration = s_sectionTimeEnd[tag] - s_sectionTimeStart[tag];

			s_sectionTimeStart.erase(tag); // Erase the start and end time points with the tag from the maps
			s_sectionTimeEnd.erase(tag);

			return s_sectionTimeDuration.count(); // Return the time taken
		}
	}
}