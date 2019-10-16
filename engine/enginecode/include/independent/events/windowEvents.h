/** \file windowEvents.h
*/

#pragma once

#include "events/event.h"

namespace Engine
{
	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}

		static EventType getStaticType() { return EventType::WindowClose; }
		EventType getEventType() const override { return EventType::WindowClose; }
		int getCategoryFlags() const override { return EventCategoryWindow; }
	};

	class WindowResizeEvent : public Event
	{
	private:
		int m_iWidth;
		int m_iHeight;
	public:
		WindowResizeEvent(int width, int height) : m_iWidth(width), m_iHeight(height) {}

		static EventType getStaticType() { return EventType::WindowResize; }
		EventType getEventType() const override { return EventType::WindowResize; }
		int getCategoryFlags() const override { return EventCategoryWindow; }

		inline int getWidth() const { return m_iWidth; }
		inline int getHeight() const { return m_iHeight; }
	};


	class WindowFocusEvent : public Event
	{
	public:
		WindowFocusEvent() {}

		static EventType getStaticType() { return EventType::WindowFocus; }
		EventType getEventType() const override { return EventType::WindowFocus; }
		int getCategoryFlags() const override { return EventCategoryWindow; }
	};


	class WindowLostFocusEvent : public Event
	{
	public:
		WindowLostFocusEvent() {}

		static EventType getStaticType() { return EventType::WindowLostFocus; }
		EventType getEventType() const override { return EventType::WindowLostFocus; }
		int getCategoryFlags() const override { return EventCategoryWindow; }
	};


	class WindowMovedEvent : public Event
	{
	public:
		WindowMovedEvent() {}

		static EventType getStaticType() { return EventType::WindowMoved; }
		EventType getEventType() const override { return EventType::WindowMoved; }
		int getCategoryFlags() const override { return EventCategoryWindow; }
	};
}