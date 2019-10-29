/** \file GLFWWindowsSystem.h
*/
#pragma once

#include "systems/windows.h"

namespace Engine
{
	/**
	\class GLFWWindowsSystem
	\brief GLFW specific WindowsSystem class, used to initialize and terminate glfw
	*/
	class GLFWWindowsSystem : public WindowsSystem
	{
	private:
		static bool ms_bInstanceFlag; //!< A boolean for if the system is running
		static std::shared_ptr<GLFWWindowsSystem> ms_pThis; //!< Pointer to the system

		bool m_bGLFWInitialized; //!< A boolean for if glfw is initialized
	public:
		//! Creates an instance of the class and returns a pointer to it
		/*!
		\return A pointer to the system
		*/
		static std::shared_ptr<GLFWWindowsSystem> getInstance(); 

		void start(SystemSignal init, ...) override; //!< Start the system
		void stop(SystemSignal close, ...) override; //!< Stop the system
	};
}