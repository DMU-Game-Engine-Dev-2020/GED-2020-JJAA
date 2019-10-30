#include "engine_pch.h"
#include "platform/GLFW/GLFWWindowsSystem.h"
#include "core/core.h"

#include <GLFW/glfw3.h>

namespace Engine
{
	bool GLFWWindowsSystem::ms_bInstanceFlag = false; // No instance to begin with
	std::shared_ptr<GLFWWindowsSystem> GLFWWindowsSystem::ms_pThis = nullptr; // Not pointing at anything

	std::shared_ptr<GLFWWindowsSystem> GLFWWindowsSystem::getInstance()
	{
		// If there is no instance of the object
		if (!ms_bInstanceFlag)
		{
			// Create a new object
			ms_pThis = std::shared_ptr<GLFWWindowsSystem>(new GLFWWindowsSystem());
			ms_bInstanceFlag = true; // There is an instance of the object

			return ms_pThis; // Return the pointer to the object
		}
		else // If the object already exists
			return ms_pThis; // Return the pointer to the object
	}

	void GLFWWindowsSystem::start(SystemSignal init, ...)
	{
		int success = glfwInit(); // Initialize GLFW
		// If initializing glfw is unsuccessful, log an error and throw an exception
		ENGINE_ASSERT(success, "Could not initialise GLFW");

		m_bGLFWInitialized = true; // glfw has been initialized
	}

	void GLFWWindowsSystem::stop(SystemSignal close, ...)
	{
		glfwTerminate(); // Terminate glfw
		m_bGLFWInitialized = false; // glfw is not initialized

		ms_bInstanceFlag = false; // No longer an instance
		ms_pThis = nullptr; // Not pointing to anything
	}
}