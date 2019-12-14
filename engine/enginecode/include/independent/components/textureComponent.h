#pragma once

#include "CGO.h"
#include "oscilateComponent.h"

#include "rendering/texture.h"

namespace Engine
{
	class TextureComponent : public Component
	{
	private:
		std::shared_ptr<Texture> m_numberTex;
		std::shared_ptr<Texture> m_letterTex;

		unsigned int m_iTexSlot;
	public:
		TextureComponent(std::shared_ptr<Texture> letter, std::shared_ptr<Texture> number) : 
			m_numberTex(number), m_letterTex(letter) {}

		void receiveMessage(const ComponentMessage& msg) override
		{
			switch (msg.m_msgType)
			{
			case ComponentMessageType::TextureSet:
				OscilateComponent::state recData = *(OscilateComponent::state*)msg.m_msgData;

				if (recData == OscilateComponent::state::DOWN)
					m_iTexSlot = m_letterTex->getSlot();
				else if (recData == OscilateComponent::state::UP)
					m_iTexSlot = m_numberTex->getSlot();

				std::pair<std::string, void*> sendData("u_texData", (void*)&m_iTexSlot);
				ComponentMessage msg(ComponentMessageType::UniformSet, (void*)&sendData);
				sendMessage(msg);
				return;
			}
		}

		void onUpdate(float timestep) override
		{
			std::pair<std::string, void*> data("u_texData", (void*)&m_iTexSlot);
			ComponentMessage msg(ComponentMessageType::UniformSet, (void*)&data);
			sendMessage(msg);
		}
	};
}