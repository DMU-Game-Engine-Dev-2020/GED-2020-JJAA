#include "engine_pch.h"
#include "platform/GLFW/GLFWWindowsSystem.h"
#include "core/core.h"

#include <GLFW/glfw3.h>

namespace Engine
{
	bool GLFWWindowsSystem::s_bInstanceFlag = false; // No instance to begin with
	std::shared_ptr<GLFWWindowsSystem> GLFWWindowsSystem::s_pThis = nullptr; // Not pointing at anything

	std::shared_ptr<GLFWWindowsSystem> GLFWWindowsSystem::getInstance()
	{
		// If there is no instance of the object
		if (!s_bInstanceFlag)
		{
			// Create a new object
			s_pThis = std::shared_ptr<GLFWWindowsSystem>(new GLFWWindowsSystem());
			s_bInstanceFlag = true; // There is an instance of the object

			return s_pThis; // Return the pointer to the object
		}
		else // If the object already exists
			return s_pThis; // Return the pointer to the object
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

		s_bInstanceFlag = false; // No longer an instance
		s_pThis = nullptr; // Not pointing to anything
	}
}