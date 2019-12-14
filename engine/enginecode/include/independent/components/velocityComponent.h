#pragma once

#include "CGO.h"

#include <glm/glm.hpp>

namespace Engine
{
	class VelocityComponent : public Component
	{
	private:
		glm::vec3 m_linear;
		glm::vec3 m_angular;

		void sendMessage(const ComponentMessage& msg) override
		{
			for (auto it = m_pOwner->begin(); it != m_pOwner->end(); it++)
			{
				(*it)->receiveMessage(msg);
			}
		}
	public:
		VelocityComponent(glm::vec3 linear, glm::vec3 angular) : m_linear(linear), m_angular(angular) {}

		void receiveMessage(const ComponentMessage& msg) override
		{
			switch (msg.m_msgType)
			{
			case ComponentMessageType::VelocitySetLinear:
				glm::vec3 linear = *(glm::vec3*)msg.m_msgData;
				m_linear = linear;
				return;
			case ComponentMessageType::VelocitySetAngular:
				glm::vec3 angular = *(glm::vec3*)msg.m_msgData;
				m_angular = angular;
				return;
			}
		}

		void onUpdate(float timestep) override
		{
			std::pair<glm::vec3, glm::vec3> data(m_linear * timestep, m_angular * timestep);
			sendMessage(ComponentMessage(ComponentMessageType::PositionIntegrate, (void*)&data));
		}
	};
}