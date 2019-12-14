#pragma once

#include "CGO.h"

namespace Engine
{
	class ControllerComponent : public Component
	{
	protected:
		void sendMessage(const ComponentMessage& msg) override
		{
			for (auto it = m_pOwner->begin(); it != m_pOwner->end(); it++)
			{
				(*it)->receiveMessage(msg);
			}
		}
	};
}