#include "engine_pch.h"
#include "systems/log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Engine
{
	std::shared_ptr<spdlog::logger> Log::s_pLogger = nullptr;
	bool Log::s_bRunning = false;

	void Log::start()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_pLogger = spdlog::stdout_color_mt("Log");
		s_pLogger->set_level(spdlog::level::trace);

		s_bRunning = true;
	}

	void Log::stop()
	{
		s_pLogger = nullptr;
		s_bRunning = false;
	}
}