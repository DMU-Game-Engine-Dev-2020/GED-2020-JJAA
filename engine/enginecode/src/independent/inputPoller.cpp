/** \file inputPoller.cpp
*/
#include "engine_pch.h"
#include "windows/inputPoller.h"
#include "platform/GLFW/GLFWInputPoller.h"
#include "core/core.h"

namespace Engine
{
#ifdef NG_PLATFORM_WINDOWS
	InputPoller* InputPoller::s_pInstance = new GLFWInputPoller(); // Create an instance of the object
#endif // NG_PLATFORM_WINDOWS
}