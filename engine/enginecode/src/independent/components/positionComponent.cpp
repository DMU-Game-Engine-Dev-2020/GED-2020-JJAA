/** \file positionComponent.cpp
*/
#include "engine_pch.h"

#include "components/positionComponent.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Engine
{
	PositionComponent::PositionComponent(glm::vec3 trans, glm::vec3 rot, glm::vec3 scale) : 
		m_transVec(trans), m_rotVec(rot), m_scaleVec(scale), m_model(glm::mat4(1.f))
	{
		// Change the rotation matrix passed in to radians and then calculate the model
		m_rotVec.x = glm::radians(m_rotVec.x);
		m_rotVec.y = glm::radians(m_rotVec.y);
		m_rotVec.z = glm::radians(m_rotVec.z);
		calculateModel();
	}

	void PositionComponent::onAttach(GameObject* owner)
	{
		// Set the owner to the one passed in
		m_pOwner = owner;
		// Send the model with the uniform name to upload
		std::pair<std::string, void*> data("u_model", (void*)&m_model[0][0]);
		ComponentMessage msg(ComponentMessageType::UniformSet, (void*)&data);
		sendMessage(msg);
	}

	void PositionComponent::onUpdate(float timestep)
	{
		// Send the model with the uniform name to upload
		std::pair<std::string, void*> data("u_model", (void*)&m_model[0][0]);
		ComponentMessage msg(ComponentMessageType::UniformSet, (void*)&data);
		sendMessage(msg);
	}

	void PositionComponent::receiveMessage(const ComponentMessage& msg)
	{
		MessageDispatcher dispatcher(msg);

		dispatcher.dispatch(ComponentMessageType::PositionSet, std::bind(&PositionComponent::positionSet, this, std::placeholders::_1));
		dispatcher.dispatch(ComponentMessageType::PositionIntegrate, std::bind(&PositionComponent::positionIntegrate, this, std::placeholders::_1));
	}

	void PositionComponent::calculateModel()
	{
		// Calculate a translation matrix
		m_translation = glm::translate(glm::mat4(1.0f), m_transVec);
		// Calculate a rotation matrix
		m_rotation = glm::toMat4(glm::quat(m_rotVec));
		// Calculate a scale matrix
		m_scale = glm::scale(glm::mat4(1.0f), m_scaleVec);
		m_model = m_translation * m_rotation * m_scale; // Calculate the model
	}

	void PositionComponent::positionSet(void* data)
	{
		glm::vec3 pos = *(glm::vec3*)data; // Get the data from the message
		m_transVec = pos; // Set the position
		calculateModel(); // Calculate the model
	}

	void PositionComponent::positionIntegrate(void* data)
	{
		// Get the data from the message
		std::pair<glm::vec3, glm::vec3> vel = *(std::pair<glm::vec3, glm::vec3>*)data;
		// Calculate new translation and rotation
		m_transVec += vel.first;
		m_rotVec.x += glm::radians(vel.second.x);
		m_rotVec.y += glm::radians(vel.second.y);
		m_rotVec.y += glm::radians(vel.second.z);
		calculateModel(); // Calculate the model
	}

	void PositionComponent::sendMessage(const ComponentMessage& msg)
	{
		// Send the message to all components on the object
		for (auto it = m_pOwner->begin(); it != m_pOwner->end(); it++)
		{
			(*it)->receiveMessage(msg);
		}
	}
}