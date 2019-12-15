/** \file textureComponent.h
*/
#pragma once

#include "CGO.h"

#include "rendering/texture.h"

namespace Engine
{
	/**
	\class TextureComponent
	\brief Component to give a gameobject a texture
	*/
	class TextureComponent : public Component
	{
	private:
		std::shared_ptr<Texture> m_tex; //!< Pointer to the texture

		unsigned int m_iTexSlot; //!< The texture slot

		//! Function to send a message to another component on the gameobject
		/*!
		\param msg The message being sent
		*/
		void sendMessage(const ComponentMessage& msg) override;
	public:
		//! Constructor
		/*!
		\param tex Pointer to the texture
		*/
		TextureComponent(std::shared_ptr<Texture> tex) : m_tex(tex) {}

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
		void receiveMessage(const ComponentMessage& msg) override {}
	};
}