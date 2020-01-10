#pragma once
#include <gtest/gtest.h>
#define private public

#include "events/event.h"
#include "events/keyEvents.h"

#include "platform/GLFW/glfwWindow.h"



//Engine::GLFWWindowImpl* window = new Engine::GLFWWindowImpl(Engine::WindowProperties());

bool worked = false;

void eventCallbackFunc(Engine::Event& e)
{
	worked = true;
}
