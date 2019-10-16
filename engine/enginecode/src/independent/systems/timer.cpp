/** \file timer.cpp
*/

#include "engine_pch.h"
#include "systems/timer.h"

namespace Engine
{
	bool Timer::ms_bInstanceFlag = false; // Initialise instance flag as false
	std::shared_ptr<Timer> Timer::ms_pThis = nullptr; // Initialise pointer to this object as null

	std::chrono::high_resolution_clock::time_point Timer::ms_frameStart;
	std::chrono::high_resolution_clock::time_point Timer::ms_frameEnd;
	std::chrono::duration<float> Timer::ms_frameDuration;

	std::map<std::string, std::pair<std::chrono::high_resolution_clock::time_point, std::chrono::high_resolution_clock::time_point>> Timer::ms_sectionTimer;
	std::chrono::duration<float> Timer::ms_sectionTimeDuration;

	bool Timer::ms_bWorking = true; // Initialise the timer as working

	std::shared_ptr<Timer> Timer::getInstance()
	{
		// If there is no instance of the object
		if (!ms_bInstanceFlag)
		{
			// Create a new object
			ms_pThis = std::shared_ptr<Timer>(new Timer());
			ms_bInstanceFlag = true; // There is an instance of the object

			return ms_pThis; // Return the pointer to the object
		}
		else // If the object already exists
			return ms_pThis; // Return the pointer to the object
	}

	void Timer::start(SystemSignal init, ...)
	{
		// Get the time now for calculating the time for a frame
		ms_frameStart = std::chrono::high_resolution_clock::now();
	}

	void Timer::stop(SystemSignal close, ...)
	{
		// No longer an instance of the object
		ms_bInstanceFlag = false;
		ms_pThis = nullptr; // Set the pointer to itself to null
	}

	void Timer::newFrame()
	{
		ms_frameEnd = std::chrono::high_resolution_clock::now(); // Get the time for the end of the frame
		ms_frameDuration = ms_frameEnd - ms_frameStart; // Calculate the time taken for the frame
		ms_frameStart = std::chrono::high_resolution_clock::now(); // Get the time for the start of the next frame
	}

	void Timer::startTimer(std::string tag)
	{
		// If the map doesn't contain the new tag
		if (ms_sectionTimer.find(tag) == ms_sectionTimer.end())
		{
			ms_sectionTimer.insert(std::make_pair(tag, std::make_pair(std::chrono::high_resolution_clock::now(), std::chrono::high_resolution_clock::now())));
		}
		else // If the map already contains the tag
		{
			// Display error using the logger
			LOG_ERROR("Timer with tag: '{0}' already exists, cannot have two timers with the same tag", tag);
			ms_bWorking = false; // Used to inform the user that timer outputs may be incorrect because multiple timers with the same tag were started
		}
	}

	float Timer::endTimer(std::string tag)
	{
		// If the map doesn't contain the tag
		if (ms_sectionTimer.find(tag) == ms_sectionTimer.end())
		{
			// Display error using the logger
			LOG_ERROR("Timer with tag: '{0}' not found, please ensure the same tag is used in start and end timer functions", tag);
			return 0.f; // Return nothing
		}
		else // If the map contains the tag
		{
			// If there has been an error
			if (!ms_bWorking)
				LOG_WARN("Two timers with the same tag were started, output may be inaccurate"); // Warn the user

			// Set the end time point for the timer
			ms_sectionTimer[tag].second = std::chrono::high_resolution_clock::now();
			// Calculate the timer duration
			ms_sectionTimeDuration = ms_sectionTimer[tag].second - ms_sectionTimer[tag].first;

			ms_sectionTimer.erase(tag); // Erase the start and end time points with the tag from the maps

			return ms_sectionTimeDuration.count(); // Return the time taken
		}
	}
}