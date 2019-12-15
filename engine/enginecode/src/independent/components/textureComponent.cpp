/** \file textureComponent.cpp
*/
#include "engine_pch.h"

#include "components/textureComponent.h"

namespace Engine
{
	void TextureComponent::onAttach(GameObject* owner)
	{
		// Set the owner
		m_pOwner = owner;
		// Set the texture slot
		m_iTexSlot = m_tex->getSlot();
		// Send the texture slot with the uniform name to upload
		std::pair<std::string, void*> data("u_texData", (void*)&m_iTexSlot);
		ComponentMessage msg(ComponentMessageType::UniformSet, (void*)&data);
		sendMessage(msg);
	}
	
	void TextureComponent::onUpdate(float timestep)
	{
		// Send the texture slot with the uniform name to upload
		std::pair<std::string, void*> data("u_texData", (void*)&m_iTexSlot);
		ComponentMessage msg(ComponentMessageType::UniformSet, (void*)&data);
		sendMessage(msg);
	}

	void TextureComponent::sendMessage(const ComponentMessage& msg)
	{
		// Send the message to all components on the object
		for (auto it = m_pOwner->begin(); it != m_pOwner->end(); it++)
		{
			(*it)->receiveMessage(msg);
		}
	}
}