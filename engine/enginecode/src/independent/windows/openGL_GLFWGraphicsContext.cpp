#include "engine_pch.h"
#include "windows/openGL_GLFWGraphicsContext.h"

void OpenGL_GLFWGraphicsContext::init()
{
	glfwMakeContextCurrent(m_pWindow);
	int result = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	
}

void OpenGL_GLFWGraphicsContext::swapBuffers()
{
	glfwSwapBuffers(m_pWindow);
}