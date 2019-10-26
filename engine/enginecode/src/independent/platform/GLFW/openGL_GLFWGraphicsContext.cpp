/** \file openGL_GLFWGraphicsContext.cpp
*/
#include "engine_pch.h"
#include "platform/GLFW/openGL_GLFWGraphicsContext.h"
#include "core/core.h"

void OpenGL_GLFWGraphicsContext::init()
{
	
	glfwMakeContextCurrent(m_pWindow);
	int result = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	ENGINE_ASSERT(result, "Could not initialize GLAD");
}

void OpenGL_GLFWGraphicsContext::swapBuffers()
{
	glfwSwapBuffers(m_pWindow);
}