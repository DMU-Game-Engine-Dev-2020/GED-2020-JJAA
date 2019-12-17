/** \file materialComponent.h
*/
#pragma once

#include "CGO.h"
#include "rendering/material.h"

#include "systems/timer.h"

namespace Engine
{
	/**
	\class MaterialComponent
	\brief Component to give a gameobject a material
	*/
	class MaterialComponent : public Component
	{
	private:
		std::shared_ptr<Material> m_pMaterial; //!< Pointer to the material

		//! Function to set a data element when receiving a UniformUpload message
		/*!
		\param data The uniform data to be set
		*/
		void uniformUpload(void* data)
		{
			// Get the data from the message
			std::pair<std::string, void*> castData = *(std::pair<std::string, void*>*)data;
			m_pMaterial->setDataElement(castData.first, castData.second); // Set the data element in the material
		}
	public:
		//! Constructor
		/*!
		\param material A pointer to the material
		*/
		MaterialComponent(const std::shared_ptr<Material>& material) : m_pMaterial(material) {}

		//! Function to get the materaial
		/*!
		\return The material
		*/
		inline std::shared_ptr<Material> getMaterial() { return m_pMaterial; }

		//! Function to receive a message from another component on the gameobject
		/*!
		\param msg The message being received
		*/
		void receiveMessage(const ComponentMessage& msg) override
		{
			MessageDispatcher dispatcher(msg);

			dispatcher.dispatch(ComponentMessageType::UniformSet, std::bind(&MaterialComponent::uniformUpload, this, std::placeholders::_1));
		}
	};
}
