#pragma once
#include <gtest/gtest.h>

#include "events/event.h"
#include "events/keyEvents.h"

#include "platform/GLFW/glfwWindow.h"

#define private public

Engine::GLFWWindowImpl* window = new Engine::GLFWWindowImpl(Engine::WindowProperties());

bool worked = false;

void eventCallbackFunc(Engine::Event& e)
{
	worked = true;
}
