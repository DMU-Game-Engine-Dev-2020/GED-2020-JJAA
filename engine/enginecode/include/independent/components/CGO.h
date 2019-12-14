#pragma once

#include "events/event.h"

namespace Engine
{
	enum class ComponentMessageType
	{
		None = 0,
		PositionIntegrate, PositionSet, RelativePositionSet,
		VelocityIntegrate, VelocitySetLinear, VelocitySetAngular,
		AccelerationSet,
		KeyPressed, KeyReleased, MouseButton, MouseScroll, MouseMoved,
		UniformSet, TextureSet
	};

	class ComponentMessage
	{
	public:
		ComponentMessage(ComponentMessageType type, void* data) : m_msgType(type), m_msgData(data) {}

		ComponentMessageType m_msgType;
		void* m_msgData;
	};

	class GameObject;

	class Component
	{
	protected:
		GameObject* m_pOwner = nullptr;

		virtual void sendMessage(const ComponentMessage& msg) {}
	public:
		virtual void onAttach(GameObject* owner) { m_pOwner = owner; }
		virtual void onDetach() { m_pOwner = nullptr; }
		virtual void onUpdate(float timestep) {}
		virtual void onEvent(Event& e) {}
		virtual void receiveMessage(const ComponentMessage& msg) = 0;
	};

	class GameObject
	{
	protected:
		std::vector<std::shared_ptr<Component>> m_components;
	public:
		void onUpdate(float timestep)
		{
			for (auto& comp : m_components)
				comp->onUpdate(timestep);
		}

		void onEvent(Event& e)
		{
			for (auto& comp : m_components)
				comp->onEvent(e);
		}

		void addComponent(const std::shared_ptr<Component> comp)
		{
			m_components.push_back(comp);
			m_components.back()->onAttach(this);
		}

		void removeComponent(std::vector<std::shared_ptr<Component>>::iterator iter);

		template <typename G>
		std::vector<std::shared_ptr<Component>>::iterator getComponent()
		{
			auto result = m_components.end();
			for (auto it = m_components.begin(); it != m_components.end(); ++it)
			{
				if (typeid(decltype(*(it->get()))).hash_code() == typeid(G).hash_code())
					return it;
			}

			return result;
		}

		inline std::vector<std::shared_ptr<Component>>::iterator begin() { return m_components.begin(); }
		inline std::vector<std::shared_ptr<Component>>::iterator end() { return m_components.end(); }
	};
}
