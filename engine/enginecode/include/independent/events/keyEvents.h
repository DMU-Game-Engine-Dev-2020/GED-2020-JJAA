/** \file keyEvents.h
*/

#pragma once

#include "events/event.h"

namespace Engine
{
	/**
	\class KeyEvent
	\brief 
	*/
	class KeyEvent : public Event
	{
	protected:
		int m_iKeyCode;

		KeyEvent(int keyCode) : m_iKeyCode(keyCode) {}
	public:
		inline int getCeyCode() const { return m_iKeyCode; }

		int getCategoryFlags() const override { return EventCategoryInput | EventCategoryKeyboard; }
	};



	class KeyPressedEvent : public KeyEvent
	{
	private:
		int m_iRepeated;
	public:
		KeyPressedEvent(int keyCode, int repeatCount) : KeyEvent(keyCode), m_iRepeated(repeatCount) {}

		static EventType getStaticType() { return EventType::KeyPressed; }
		EventType getEventType() const override { return EventType::KeyPressed; }

		inline bool getRepeated() const { return m_iRepeated; }
	};


	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keyCode) : KeyEvent(keyCode) {}

		static EventType getStaticType() { return EventType::KeyReleased; }
		EventType getEventType() const override { return EventType::KeyReleased; }
	};


	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int keyCode) : KeyEvent(keyCode) {}

		static EventType getStaticType() { return EventType::KeyTyped; }
		EventType getEventType() const override { return EventType::KeyTyped; }
	};
}