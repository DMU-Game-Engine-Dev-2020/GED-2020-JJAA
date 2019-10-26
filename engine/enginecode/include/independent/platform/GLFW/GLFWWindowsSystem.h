#pragma once

#include "systems/windows.h"

namespace Engine
{
	class GLFWWindowsSystem : public WindowsSystem
	{
	private:
		static bool ms_bInstanceFlag;
		static std::shared_ptr<GLFWWindowsSystem> ms_pThis;

		bool m_bGLFWInitialized;
	public:
		static std::shared_ptr<GLFWWindowsSystem> getInstance();

		void start(SystemSignal init, ...) override;
		void stop(SystemSignal close, ...) override;
	};
}