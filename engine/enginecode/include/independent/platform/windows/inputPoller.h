/** \file inputPoller.h
*/

#pragma once

#include <glm/glm.hpp>
#include <memory>

namespace Engine
{
	/**
	\class InputPoller
	\brief Base class for finding the state of inputs at any point in the program
	*/
	class InputPoller
	{
	private:
		static InputPoller* s_pInstance; //!< Pointer for singleton instance
	protected:
		virtual bool isKeyPressedImpl(int keycode) = 0; //!< Windows API specific implementation of key press
		virtual bool isMouseButtonPressedImpl(int button) = 0; //!< Windows API specific implementation of mouse button press
		virtual glm::vec2 getMousePositionImpl() = 0; //!< Windows API specific implementation of mouse position
		virtual float getMouseXImpl() = 0; //!< Windows API specific implementation of mouse x position
		virtual float getMouseYImpl() = 0; //!< Windows API specific implementation of mouse y postion
	public:
		inline static bool isKeyPressed(int keycode) { return s_pInstance->isKeyPressedImpl(keycode); } //!< Return if the key with the key code is pressed
		inline static bool isMouseButtonPressed(int button) { return s_pInstance->isMouseButtonPressedImpl(button); } //!< Return if the mouse button is pressed
		inline static glm::vec2 getMousePosition() { return s_pInstance->getMousePositionImpl(); } //!< Return the current position of the mouse
		inline static float getMouseX() { return s_pInstance->getMouseXImpl(); } //!< Return the current x position of the mouse
		inline static float getMouseY() { return s_pInstance->getMouseYImpl(); } //!< Return the current y position of the mouse
	};
}