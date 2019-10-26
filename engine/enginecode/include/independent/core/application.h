/** \file application.h
*/
#pragma once

#include "systems/log.h"
#include "systems/timer.h"

#include "platform/windows/window.h"
#include "GLFWCodes.h"

namespace Engine {

	/**
	\class Application
	Fundemental class of the engine. A singleton which runs the game loop infinitely.
	Provides ...
	*/

	class Application
	{
	protected:
		Application(); //!< Constructor
	private:
		static Application* ms_instance; //!< Singleton instance of the application

		std::shared_ptr<Log> m_pLogger; //!< Pointer to the Log system object
		std::shared_ptr<Timer> m_pTimer; //!< Pointer to the Timer system object

		bool m_bRunning; //!< If the application is running
		float m_fTotalTimeElapsed; //!< The total time the program has been active in seconds
		float m_fTimestep; //!< Time in seconds for the last frame

		std::unique_ptr<Window> m_pWindow; //!< Unique pointer to the window


#pragma region TempVars
// Temp stuff
		unsigned int m_FCvertexArray; // Flat Colour VAO
		unsigned int m_FCvertexBuffer;// Flat Colour VBO
		unsigned int m_TPvertexArray; // Textured Phong VAO
		unsigned int m_TPvertexBuffer;// Textured Phong VBO
		unsigned int m_FCindexBuffer; // Index buffer for colour cube
		unsigned int m_TPindexBuffer; // Index buffer for texture Phong cube
		unsigned int m_FCprogram; // Flat colour shader ID
		unsigned int m_TPprogram; // Textured Phong shader ID
		unsigned int m_numberTexture; // Texture ID
		unsigned int m_letterTexture; // Texture ID
		unsigned int m_textureSlots[2]; // Slot where the textures are stored
		bool m_goingUp = false; // Is the cude going up?
		float m_timeSummed = 10.f; // How much timer has elasped?
#pragma endregion TempVars

	public:
		virtual ~Application(); //!< Deconstructor
		inline static Application& getInstance() { return *ms_instance; } //!< Instance getter from singleton pattern
		void onEvent(Event& e); //!< When an event happens
		bool onClose(WindowCloseEvent& e); //!< Called if the event is a window close event
		bool onResize(WindowResizeEvent& e); //!< Called if the event is a window resize event
		bool onFocus(WindowFocusEvent& e); //!< Called if the window gets focus
		bool onLostFocus(WindowLostFocusEvent& e); //!< Callsed if the window loses focus
		bool onKeyPressed(KeyPressedEvent& e);
		void run(); //!< Main loop
		inline Window& getWindow() { return *m_pWindow; } //!< Returns window
	};

	// To be defined in users code
	Application* startApplication(); //!< Function definition which provides an entry hook

}