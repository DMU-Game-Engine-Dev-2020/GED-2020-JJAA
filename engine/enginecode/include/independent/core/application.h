/** \file application.h
*/
#pragma once

#include "systems/log.h"
#include "systems/timer.h"
#include "events/windowEvents.h"

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
		float m_fLastFrameTime; //!< Time in seconds for the last frame
	public:
		virtual ~Application(); //!< Deconstructor
		inline static Application& getInstance() { return *ms_instance; } //!< Instance getter from singleton pattern
		void onEvent(Event& e); //!< When an event happens
		bool onClose(WindowCloseEvent& e); //!< Called if the event is a window close event
		bool onResize(WindowResizeEvent& e); //!< Called if the event is a window resize event
		void run(); //!< Main loop
	};

	// To be defined in users code
	Application* startApplication(); //!< Function definition which provides an entry hook

}