#pragma once

#include "CGO.h"
#include "rendering/material.h"

namespace Engine
{
	class MaterialComponent : public Component
	{
	private:
		std::shared_ptr<Material> m_pMaterial;
	public:
		MaterialComponent(const std::shared_ptr<Material>& material) : m_pMaterial(material) {}

		inline std::shared_ptr<Material> getMaterial() { return m_pMaterial; }

		void receiveMessage(const ComponentMessage& msg) override
		{
			switch (msg.m_msgType)
			{
			case ComponentMessageType::UniformSet:
				std::pair<std::string, void*> data = *(std::pair<std::string, void*>*)msg.m_msgData;
				m_pMaterial->setDataElement(data.first, data.second);
				return;
			}
		}
	};
}
