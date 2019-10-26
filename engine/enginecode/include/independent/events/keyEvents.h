/** \file keyEvents.h
\brief Contains classes used for key events
*/

#pragma once

#include "events/event.h"

namespace Engine
{
	/**
	\class KeyEvent
	\brief Base class inheriting from Event, for all key events

	Contains common attributes for all key events
	*/
	class KeyEvent : public Event
	{
	protected:
		int m_iKeyCode; //!< Identifying number for which key is being used

		KeyEvent(int keyCode) : m_iKeyCode(keyCode) {} //!< Constructor, sets stored key code to the one passed in. Protected so only classes inheriting from the class can create it
	public:
		inline int getKeyCode() const { return m_iKeyCode; } //!< Returns the key code

		int getCategoryFlags() const override { return EventCategoryInput | EventCategoryKeyboard; } //!< Returns the category flags
	};


	/**
	\class KeyPressedEvent
	\brief Event for if a key has been pressed
	*/
	class KeyPressedEvent : public KeyEvent
	{
	private:
		int m_iRepeated; //!< If the key press is a repeat and if so how many times has it been pressed
	public:
		KeyPressedEvent(int keyCode, int repeatCount) : KeyEvent(keyCode), m_iRepeated(repeatCount) {} //!< Constructor

		static EventType getStaticType() { return EventType::KeyPressed; } //!< Returns the type even if the event has not been created
		EventType getEventType() const override { return EventType::KeyPressed; } //!< Returns the event type

		inline bool getRepeated() const { return m_iRepeated; } //!< Returns the numer of times the key has been repeatedly pressed
	};

	/**
	\class KeyReleasedEvent
	\brief Event for if a key has been released
	*/
	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keyCode) : KeyEvent(keyCode) {} //!< Constructor

		static EventType getStaticType() { return EventType::KeyReleased; } //!< Returns the type even if the event has not been created
		EventType getEventType() const override { return EventType::KeyReleased; } //!< Returns the event type
	};

	/**
	\class KeyTypedEvent
	\brief Event for if a key has been typed
	*/
	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int keyCode) : KeyEvent(keyCode) {} //!< Constructor

		static EventType getStaticType() { return EventType::KeyTyped; } //!< Returns the type even if the event has not been created
		EventType getEventType() const override { return EventType::KeyTyped; } //!< Returns the event type
	};
}