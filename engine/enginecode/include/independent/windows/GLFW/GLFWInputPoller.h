/** \file GLFWInputPoller.h
*/

#pragma once

#include "windows/inputPoller.h"

namespace Engine
{
	/**
	\class GLFWInputPoller
	\brief GLFW API specific input polling
	*/
	class GLFWInputPoller : public InputPoller
	{
	protected:
		bool isKeyPressedImpl(int keycode) override; //!< Windows API specific implementation of key press
		bool isMouseButtonPressedImpl(int button) override; //!< Windows API specific implementation of mouse button press
		glm::vec2 getMousePositionImpl() override; //!< Windows API specific implementation of mouse position
		float getMouseXImpl() override; //!< Windows API specific implementation of mouse x position
		float getMouseYImpl() override; //!< Windows API specific implementation of mouse y postion
	};
}