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
		// Create message dispatcher
		MessageDispatcher dispatcher(msg);
		// If the message type matches, call the corresponding function
		dispatcher.dispatch(ComponentMessageType::TextureSet, std::bind(&TextureSwitchComponent::textureSet, this, std::placeholders::_1));
	}

	void TextureSwitchComponent::textureSet(void* data)
	{
		OscilateComponent::state tex = *(OscilateComponent::state*)data; // Get the data from the message

			// If the state is up
		if (tex == OscilateComponent::state::UP)
		{
			// Make the current texture slot the one from the first texture
			m_iTexSlot = m_tex1->getSlot();
		}
		else if (tex == OscilateComponent::state::DOWN) // If the state is down
		{
			// Make the current texture slot the one from the second texture
			m_iTexSlot = m_tex2->getSlot();
		}
	}
}