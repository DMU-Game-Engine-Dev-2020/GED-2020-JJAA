/** \file windows.h
*/
#pragma once

#include "system.h"

namespace Engine
{
	/**
	\class WindowsSystem
	\brief 
	*/
	class WindowsSystem : public System
	{
	public:
		virtual void start(SystemSignal init, ...) override = 0; //!< Inherited from System, to be implemented somewhere else
		virtual void stop(SystemSignal close, ...) override = 0; //!< Inherited from System, to be implemented somewhere else
	};
}