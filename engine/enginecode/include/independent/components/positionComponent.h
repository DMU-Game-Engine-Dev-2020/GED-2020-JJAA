/** \file positionComponent.h
*/
#pragma once

#include "CGO.h"

#include <glm/glm.hpp>

namespace Engine
{
	/**
	\class PositionComponent
	\brief Component to give a gameobject a position
	*/
	class PositionComponent : public Component
	{
	private:
		glm::mat4 m_model; //!< The object model
		glm::mat4 m_translation; //!< The position to calculate the model
		glm::mat4 m_rotation; //!< The rotation to calculate the model
		glm::mat4 m_scale; //!< The scale to calculate the model
		glm::vec3 m_transVec; //!< The position
		glm::vec3 m_rotVec; //!< The rotation
		glm::vec3 m_scaleVec; //!< The scale

		//! Funcion to calculate the model
		inline void calculateModel();

		//! Function to send a message to another component on the gameobject
		/*!
		\param msg The message being sent
		*/
		void sendMessage(const ComponentMessage& msg) override;
	public:
		//! Constructor
		/*!
		\param trans The translation vector
		\param rot The rotation vector
		\param scale
		*/
		PositionComponent(glm::vec3 trans, glm::vec3 rot, glm::vec3 scale);

		//! Function to get the objects transform
		/*!
		\return The transform of the object
		*/
		inline std::shared_ptr<glm::mat4> getTransform() { return std::make_shared<glm::mat4>(m_model); }

		//! Function called when the component is attached to a gameobject
		/*!
		\param owner Pointer to the gameobject the component is being attached to
		*/
		void onAttach(GameObject* owner) override;
		//! Function to update the component every frame
		/*!
		\param timestep The time since the previous frame
		*/
		void onUpdate(float timestep) override;
		//! Function to receive a message from another component on the gameobject
		/*!
		\param msg The message being received
		*/
		void receiveMessage(const ComponentMessage& msg) override;
	};
}