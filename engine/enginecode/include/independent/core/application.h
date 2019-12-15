/** \file application.h
*/
#pragma once

#include "systems/log.h"
#include "systems/timer.h"
#include "systems/windows.h"
#include "systems/resourceManager.h"
#include "systems/layerstack.h"

#include "windows/window.h"

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

		std::shared_ptr<Log> m_pLogger; //!< Pointer to the Log system object
		std::shared_ptr<Timer> m_pTimer; //!< Pointer to the Timer system object
		std::shared_ptr<WindowsSystem> m_pWindows; //!< Pointer to the Windows system
		std::shared_ptr<ResourceManager> m_pResources; //!< Pointer to the resource manager system
		std::shared_ptr<Layerstack> m_pLayerStack;
	private:
		static Application* s_instance; //!< Singleton instance of the application

		bool m_bRunning; //!< If the application is running
		float m_fTotalTimeElapsed; //!< The total time the program has been active in seconds
		float m_fTimestep; //!< Time in seconds for the last frame
		
		std::unique_ptr<Window> m_pWindow; //!< Unique pointer to the window
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