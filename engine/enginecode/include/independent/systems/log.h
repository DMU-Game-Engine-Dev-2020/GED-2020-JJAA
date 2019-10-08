#pragma once

#include "system.h"
#include "spdlog/spdlog.h"

namespace Engine
{
	class Log : public System
	{
	private:
		static std::shared_ptr<spdlog::logger> s_pLogger;
		static bool s_bRunning;
	public:
		static void start();
		static void stop();

		static std::shared_ptr<spdlog::logger> getLogger() { return s_pLogger; }
		static bool checkRunning() { return s_bRunning; }
	};
}

#define LOG_INFO(...)      Engine::Log::getLogger()->info(__VA_ARGS__)
#define LOG_TRACE(...)     Engine::Log::getLogger()->trace(__VA_ARGS__)
#define LOG_WARN(...)      Engine::Log::getLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)     Engine::Log::getLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...)  Engine::Log::getLogger()->critical(__VA_ARGS__)
