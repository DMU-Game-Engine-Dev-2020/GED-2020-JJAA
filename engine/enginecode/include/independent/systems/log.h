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
		static std::shared_ptr<spdlog::logger> s_pLogger; //!< A static shared pointer to the logger
		static bool s_bRunning; //!< A static boolean for if the system is running
	public:
		static void start(); //!< A static function to start the logger
		static void stop(); //!< A static function to stop the logger

		static std::shared_ptr<spdlog::logger> getLogger() { return s_pLogger; } //!< To get the logger
		static bool checkRunning() { return s_bRunning; } //!< To check if the logger is running
	};
}

// Macros to shorten function calls when logging
#define LOG_INFO(...)      Engine::Log::getLogger()->info(__VA_ARGS__)
#define LOG_TRACE(...)     Engine::Log::getLogger()->trace(__VA_ARGS__)
#define LOG_WARN(...)      Engine::Log::getLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)     Engine::Log::getLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...)  Engine::Log::getLogger()->critical(__VA_ARGS__)
