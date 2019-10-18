/** \file GLFWInputPoller.cpp
*/

#include "engine_pch.h"
#include "core/application.h"
#include "windows/GLFW/GLFWInputPoller.h"

#include <GLFW/glfw3.h>

namespace Engine
{
	InputPoller* InputPoller::s_pInstance = new GLFWInputPoller();

	bool GLFWInputPoller::isKeyPressedImpl(int keycode)
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::getInstance().getWindow().getNativeWindow());
		int result = glfwGetKey(window, keycode);
		return result == GLFW_PRESS || result == GLFW_REPEAT;
	}

	bool GLFWInputPoller::isMouseButtonPressedImpl(int button)
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::getInstance().getWindow().getNativeWindow());
		int result = glfwGetMouseButton(window, button);
		return result == GLFW_PRESS;
	}

	glm::vec2 GLFWInputPoller::getMousePositionImpl()
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::getInstance().getWindow().getNativeWindow());
		double x;
		double y;
		glfwGetCursorPos(window, &x, &y);

		return { (float)x, (float)y };

	}

	float GLFWInputPoller::getMouseXImpl()
	{
		glm::vec2 pos = getMousePositionImpl();
		return pos.x;
	}

	float GLFWInputPoller::getMouseYImpl()
	{
		glm::vec2 pos = getMousePositionImpl();
		return pos.y;
	}
}