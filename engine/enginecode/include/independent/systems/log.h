#pragma once

#include "system.h"
#include "spdlog/spdlog.h"

class Log : public Engine::System
{
private:
	static std::shared_ptr<spdlog::logger> s_pLogger;
	static bool s_bRunning;
public:
	static std::shared_ptr<spdlog::logger> getLogger() { return s_pLogger; }
	static bool checkRunning() { return s_bRunning; }
};