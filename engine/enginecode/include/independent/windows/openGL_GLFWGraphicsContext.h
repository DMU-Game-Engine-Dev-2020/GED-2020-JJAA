#pragma once

#include "graphicsContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class OpenGL_GLFWGraphicsContext : public GraphicsContext
{
private:
	GLFWwindow* m_pWindow;
public:
	OpenGL_GLFWGraphicsContext(GLFWwindow* window) : m_pWindow(window) {}
	void init() override;
	void swapBuffers() override;
};