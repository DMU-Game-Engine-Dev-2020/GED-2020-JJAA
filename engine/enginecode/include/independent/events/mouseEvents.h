/** \file mouseEvents.h
*/

#pragma once

#include "events/event.h"

namespace Engine
{
	class MouseButtonEvent : public Event
	{
	protected:
		int m_iButton;

		MouseButtonEvent(int button) : m_iButton(button) {}
	public:
		inline int getMouseButton() const { return m_iButton; }

		int getCategoryFlags() const override { return EventCategoryInput | EventCategoryMouse | EventCategoryMouseButton; }
	};



	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

		static EventType getStaticType() { return EventType::MouseButtonPressed; }
		EventType getEventType() const override { return EventType::MouseButtonPressed; }
	};


	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

		static EventType getStaticType() { return EventType::MouseButtonReleased; }
		EventType getEventType() const override { return EventType::MouseButtonReleased; }
	};


	class MouseMovedEvent : public Event
	{
	private:
		float m_fMouseX;
		float m_fMouseY;
	public:
		MouseMovedEvent(float x, float y) : m_fMouseX(x), m_fMouseY(y) {}

		static EventType getStaticType() { return EventType::MouseMoved; }
		EventType getEventType() const override { return EventType::MouseMoved; }
		int getCategoryFlags() const override { return EventCategoryInput | EventCategoryMouse; }

		inline float getX() const { return m_fMouseX; }
		inline float getY() const { return m_fMouseY; }
	};


	class MouseScrolledEvent : public Event
	{
	private:
		float m_fXOffset;
		float m_fYOffset;
	public:
		MouseScrolledEvent(float x, float y) : m_fXOffset(x), m_fYOffset(y) {}

		static EventType getStaticType() { return EventType::MouseScrolled; }
		EventType getEventType() const override { return EventType::MouseScrolled; }
		int getCategoryFlags() const override { return EventCategoryInput | EventCategoryMouse; }

		inline float getXOffset() const { return m_fXOffset; }
		inline float getYOffset() const { return m_fYOffset; }
	};
}