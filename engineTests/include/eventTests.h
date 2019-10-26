#pragma once
#include <gtest/gtest.h>

#include "events/event.h"
#include "events/windowEvents.h"
#include "events/mouseEvents.h"
#include "GLFWCodes.h"

const int width = 1024;
const int height = 720;
Engine::WindowResizeEvent re(width, height);
Engine::WindowCloseEvent ce;

const int button1 = ENGINE_MOUSE_BUTTON_1;
const int button2 = ENGINE_MOUSE_BUTTON_2;
Engine::MouseButtonPressedEvent mbpe(button1);
Engine::MouseButtonReleasedEvent mbre(button2);

bool onResizeTrue(Engine::WindowResizeEvent& e)
{
	return true;
}

bool onResizeFalse(Engine::WindowResizeEvent& e)
{
	return false;
}