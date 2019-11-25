/** \file application.h
*/
#pragma once

#include "systems/log.h"
#include "systems/timer.h"
#include "systems/windows.h"

#include "windows/window.h"
#include "windows/inputPoller.h"
#include "core/codes.h"

#include "rendering/renderer/renderer.h"

#include "systems/resourceManager.h"

#include "rendering/character.h"

namespace Engine 
{
	/**
	\class Application
	\brief Fundemental class of the engine. A singleton which runs the game loop infinitely.
	*/
	class Application
	{
	protected:
		Application(); //!< Constructor
	private:
		static Application* s_instance; //!< Singleton instance of the application

		std::shared_ptr<Log> m_pLogger; //!< Pointer to the Log system object
		std::shared_ptr<Timer> m_pTimer; //!< Pointer to the Timer system object
		std::shared_ptr<WindowsSystem> m_pWindows; //!< Pointer to the Windows system
		std::shared_ptr<ResourceManager> m_pResources; //!< Pointer to the resource manager system

		//std::shared_ptr<Texture> m_pLetterCubeTexture; //!< Pointer to the letter cube texture
		//std::shared_ptr<Texture> m_pNumberCubeTexture; //!< Pointer to the number cube texture

		//std::shared_ptr<Material> m_pFCMat; //!< Pointer to the flat colour material
		//std::shared_ptr<Material> m_pTPMat; //!< Pointer to the textured phong material

		//std::shared_ptr<UniformBuffer> m_matricesUBO;
		//std::shared_ptr<UniformBuffer> m_lightUBO;

		//std::unique_ptr<Renderer> m_pRenderer; //!< Pointer to the renderer

		bool m_bRunning; //!< If the application is running
		float m_fTotalTimeElapsed; //!< The total time the program has been active in seconds
		float m_fTimestep; //!< Time in seconds for the last frame

		std::unique_ptr<Window> m_pWindow; //!< Unique pointer to the window

		std::shared_ptr<Texture> m_pGlyphTexture;
		std::shared_ptr<Material> m_pTextMat;
		std::unique_ptr<Renderer> m_pTextRenderer;

#pragma region TempVars
// Temp stuff
		bool m_goingUp = false; //!< If a cube is going up
		float m_timeSummed = 10.f; //!< How much time has elasped?
#pragma endregion TempVars

	public:
		virtual ~Application(); //!< Deconstructor

		//! Instance getter from singleton pattern
		/*!
		\return A reference to the application instance
		*/
		inline static Application& getInstance() { return *s_instance; } 
		//! When an event happens
		/*!
		\param e A reference to the event
		*/
		void onEvent(Event& e); 
		//! Called if the event is a window close event
		/*!
		\param e A reference to the event
		\return A boolean representing if the event was successful or not
		*/
		bool onClose(WindowCloseEvent& e);
		//! Called if the event is a window resize event
		/*!
		\param e A reference to the event
		\return A boolean representing if the event was successful or not
		*/
		bool onResize(WindowResizeEvent& e); 
		//! Called if the window gets focus
		/*!
		\param e A reference to the event
		\return A boolean representing if the event was successful or not
		*/
		bool onFocus(WindowFocusEvent& e); 
		//! Called if the window loses focus
		/*!
		\param e A reference to the event
		\return A boolean representing if the event was successful or not
		*/
		bool onLostFocus(WindowLostFocusEvent& e); 
		//! Called if the window is moved
		/*!
		\param e A reference to the event
		\return A boolean representing if the event was successful or not
		*/
		bool onWindowMoved(WindowMovedEvent& e); 
		//! Called if a keyboard key is pressed
		/*!
		\param e A reference to the event
		\return A boolean representing if the event was successful or not
		*/
		bool onKeyPressed(KeyPressedEvent& e); 
		void run(); //!< Main loop

		//! Used to get the window
		/*!
		\return A reference to the current window
		*/
		inline Window& getWindow() { return *m_pWindow; }
	};

	// To be defined in users code
	Application* startApplication(); //!< Function definition which provides an entry hook

}