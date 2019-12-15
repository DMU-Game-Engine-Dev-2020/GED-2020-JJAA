/** \file materialComponent.h
*/
#pragma once

#include "CGO.h"
#include "rendering/material.h"

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
			// Do different things depending on the message type
			switch (msg.m_msgType)
			{
			case ComponentMessageType::UniformSet:
				// Get the data from the message
				std::pair<std::string, void*> data = *(std::pair<std::string, void*>*)msg.m_msgData;
				m_pMaterial->setDataElement(data.first, data.second); // Set the data element in the material
				return;
			}
		}
	};
}
