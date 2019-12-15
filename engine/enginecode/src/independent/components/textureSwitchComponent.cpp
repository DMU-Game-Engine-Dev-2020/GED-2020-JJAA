/** \file textureSwitchComponent.cpp
*/
#include "engine_pch.h"

#include "components/textureSwitchComponent.h"
#include "components/oscilateComponent.h"

namespace Engine
{
	void TextureSwitchComponent::onAttach(GameObject* owner)
	{
		// Set the owner
		m_pOwner = owner;
		// Set the texture slot
		m_iTexSlot = m_tex1->getSlot();
		// Send the texture slot with the uniform name to upload
		std::pair<std::string, void*> data("u_texData", (void*)&m_iTexSlot);
		ComponentMessage msg(ComponentMessageType::UniformSet, (void*)&data);
		sendMessage(msg);
	}

	void TextureSwitchComponent::onUpdate(float timestep)
	{
		// Send the texture slot with the uniform name to upload
		std::pair<std::string, void*> data("u_texData", (void*)&m_iTexSlot);
		ComponentMessage msg(ComponentMessageType::UniformSet, (void*)&data);
		sendMessage(msg);
	}

	void TextureSwitchComponent::receiveMessage(const ComponentMessage& msg)
	{
		// Do different things depending on the message type
		switch (msg.m_msgType)
		{
		case ComponentMessageType::TextureSet:
			OscilateComponent::state data = *(OscilateComponent::state*)msg.m_msgData; // Get the data from the message

			// If the state is up
			if (data == OscilateComponent::state::UP)
			{
				// Make the current texture slot the one from the first texture
				m_iTexSlot = m_tex1->getSlot();
			}
			else if (data == OscilateComponent::state::DOWN) // If the state is down
			{
				// Make the current texture slot the one from the second texture
				m_iTexSlot = m_tex2->getSlot();
			}
			return;
		}
	}
}