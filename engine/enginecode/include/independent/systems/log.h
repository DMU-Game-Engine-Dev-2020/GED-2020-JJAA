/** \file log.h
*/
#pragma once

#include "system.h"
#include "spdlog/spdlog.h"

namespace Engine
{
	/**
	\class Log
	A system class to be used for logging information, errors etc. to help a programmer know what is happening if
	something goes wrong.
	It is a class which provides access to the 3rd party library 'spdlog'
	*/

	class Log : public System
	{
	private:
		static bool s_bInstanceFlag; //!< A boolean for if the system is running
		static std::shared_ptr<Log> s_pThis; //!< Pointer to Log object

		static std::shared_ptr<spdlog::logger> s_pLogger; //!< Pointer to the logger
	public:
		static std::shared_ptr<Log> getInstance(); //!< Create instance of object and return a pointer to it

		void start(SystemSignal init, ...) override; //!< A function to start the logger
		void stop(SystemSignal close, ...) override; //!< A function to stop the logger

		inline static std::shared_ptr<spdlog::logger> getLogger() { return s_pLogger; } //!< To get the logger
	};
}

// Macros to shorten function calls when logging
#define LOG_INFO(...)      Engine::Log::getLogger()->info(__VA_ARGS__)
#define LOG_TRACE(...)     Engine::Log::getLogger()->trace(__VA_ARGS__)
#define LOG_WARN(...)      Engine::Log::getLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)     Engine::Log::getLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...)  Engine::Log::getLogger()->critical(__VA_ARGS__)
