#pragma once
#include <gtest/gtest.h>

#include "events/event.h"
#include "events/windowEvents.h"
#include "events/mouseEvents.h"
#include "events/keyEvents.h"

const int width = 1024;
const int height = 720;
Engine::WindowResizeEvent re(width, height);
Engine::WindowCloseEvent ce;

bool onResizeTrue(Engine::WindowResizeEvent& e)
{
	return true;
}

bool onResizeFalse(Engine::WindowResizeEvent& e)
{
	return false;
}

const int button1 = 0;
const int button2 = 1;
const float posX = 400;
const float posY = 200;
Engine::MouseButtonPressedEvent mbpe(button1);
Engine::MouseButtonReleasedEvent mbre(button2);
Engine::MouseMovedEvent mme(posX, posY);

const int keyCode = 68;
Engine::KeyPressedEvent kpe(keyCode, 0);
Engine::KeyPressedEvent krpe(keyCode, 1);