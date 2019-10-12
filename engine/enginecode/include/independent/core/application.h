/** \file application.h
*/
#pragma once

#include "systems/log.h"
#include "systems/timer.h"

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
		static Application* s_instance; //!< Singleton instance of the application

		std::shared_ptr<Log> pLog; //!< Pointer to the Log system object
		std::shared_ptr<Timer> pTimer; //!< Pointer to the Timer system object

		bool bRunning; //!< If the application is running
		float fTotalTimeElapsed; //!< The total time the program has been active in seconds
		float fLastFrameTime; //!< Time in seconds for the last frame
	public:
		virtual ~Application(); //!< Deconstructor
		inline static Application& getInstance() { return *s_instance; } //!< Instance getter from singleton pattern
		void run(); //!< Main loop
	};

	// To be defined in users code
	Application* startApplication(); //!< Function definition which provides an entry hook

}