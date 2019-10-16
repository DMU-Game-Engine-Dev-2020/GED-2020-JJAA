/** \file log.cpp
*/

#include "engine_pch.h"
#include "systems/log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Engine
{
	bool Log::ms_bInstanceFlag = false; // Initialise the instance flag as false
	std::shared_ptr<Log> Log::ms_pThis = nullptr; // Initialise the pointer to this as null

	std::shared_ptr<spdlog::logger> Log::ms_pLogger = nullptr; // Initialise the pointer to the spdlog logger as null

	std::shared_ptr<Log> Log::getInstance()
	{
		// If there is no instance of the object
		if (!ms_bInstanceFlag)
		{
			// Create a new object
			ms_pThis = std::shared_ptr<Log>(new Log());
			ms_bInstanceFlag = true; // There is an instance of the object

			return ms_pThis; // Return the pointer to the object
		}
		else // If the object already exists
			return ms_pThis; // Return the pointer to the object
	}

	void Log::start(SystemSignal init, ...)
	{
		// Set up the logger
		spdlog::set_pattern("%^[%T] %n: %v%$");
		ms_pLogger = spdlog::stdout_color_mt("Log");
		ms_pLogger->set_level(spdlog::level::trace);
	}

	void Log::stop(SystemSignal close, ...)
	{
		ms_pLogger = nullptr; // Set the pointer to the spdlog logger to null
		ms_bInstanceFlag = false; // No longer an instance to the object
		ms_pThis = nullptr; // Set pointer to itself to null
	}
}