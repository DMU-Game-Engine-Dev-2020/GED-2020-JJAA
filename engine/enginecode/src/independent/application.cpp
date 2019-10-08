/** \file application.cpp
*/


#include "engine_pch.h"
#include "core/application.h"


namespace Engine {
	Application* Application::s_instance = nullptr;

	Application::Application()
	{
		if (s_instance == nullptr)
		{
			s_instance = this;
		}

		Log::start();
		/*LOG_INFO("Hello world");
		LOG_TRACE("Trace");
		LOG_WARN("Warning: {0}", 42);
		LOG_ERROR("Error code: {0}, error message: {1}", 134, "WHY?!?!?!");
		LOG_CRITICAL("Error code: {0}, error message: {1}", 666, "That was a big mistake, good luck fixing it");*/
	}

	Application::~Application()
	{
	}

	

	void Application::run()
	{

	}

}
