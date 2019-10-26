#include "engine_pch.h"
#include "platform/GLFW/GLFWWindowsSystem.h"
#include "core/core.h"

#include <GLFW/glfw3.h>

namespace Engine
{
	bool GLFWWindowsSystem::ms_bInstanceFlag = false;
	std::shared_ptr<GLFWWindowsSystem> GLFWWindowsSystem::ms_pThis = nullptr;

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
		ENGINE_ASSERT(success, "Could not initialise GLFW");

		m_bGLFWInitialized = true;
	}

	void GLFWWindowsSystem::stop(SystemSignal close, ...)
	{
		glfwTerminate();
		m_bGLFWInitialized = false;

		ms_bInstanceFlag = false;
		ms_pThis = nullptr;
	}
}