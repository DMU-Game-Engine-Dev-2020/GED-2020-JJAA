#pragma once

#include "controllerComponent.h"

#include <glm/glm.hpp>

namespace Engine
{
	class OscilateComponent : public ControllerComponent
	{
	public:
		enum state { DOWN = -1, STOPPED = 0, UP = 1 };
	private:
		state m_state = UP;
		float m_fTimeAccumulated;
	public:
		OscilateComponent(OscilateComponent::state initialState, float timeAccumulated) : 
			m_state(initialState), m_fTimeAccumulated(timeAccumulated) {}

		void onAttach(GameObject* owner) override
		{
			m_pOwner = owner;
			
			if (m_state == DOWN)
			{
				sendMessage(ComponentMessage(ComponentMessageType::VelocitySetLinear, std::any(glm::vec3(0.f, -0.2f, 0.f))));
			}
			else if (m_state == UP)
			{
				sendMessage(ComponentMessage(ComponentMessageType::VelocitySetLinear, std::any(glm::vec3(0.f, 0.2f, 0.f))));
			}
			sendMessage(ComponentMessage(ComponentMessageType::TextureSet, std::any(m_state)));
		}

		void onUpdate(float timestep) override
		{
			m_fTimeAccumulated += timestep;

			if (m_fTimeAccumulated >= 20.f)
			{
				if (m_state == DOWN)
				{
					m_state = UP;
					sendMessage(ComponentMessage(ComponentMessageType::VelocitySetLinear, std::any(glm::vec3(0.f, 0.2f, 0.f))));
				}
				else if (m_state == UP)
				{
					m_state = DOWN;
					sendMessage(ComponentMessage(ComponentMessageType::VelocitySetLinear, std::any(glm::vec3(0.f, -0.2f, 0.f))));
				}
				sendMessage(ComponentMessage(ComponentMessageType::TextureSet, std::any(m_state)));

				m_fTimeAccumulated = 0;
			}
		}

		void receiveMessage(const ComponentMessage& msg) override
		{

		}
	};
}