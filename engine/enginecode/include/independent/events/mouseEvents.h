/** \file mouseEvents.h
\brief Contains classes used for mouse events
*/

#pragma once

#include "events/event.h"

namespace Engine
{
	/**
	\class MouseButtonEvent
	\brief Base class inheriting from Event, for all mouse button events

	Contains common attributes for all mouse button events
	*/
	class MouseButtonEvent : public Event
	{
	protected:
		int m_iButton; //!< Identifying number for which button is being used

		MouseButtonEvent(int button) : m_iButton(button) {} //!< Constructor, sets stored button to the one passed in. Protected so only classes inheriting from the class can create it
	public:
		inline int getMouseButton() const { return m_iButton; } //!< Returns the button

		int getCategoryFlags() const override { return EventCategoryInput | EventCategoryMouse | EventCategoryMouseButton; } //!< Returns the category flags
	};


	/**
	\class MouseButtonPressedEvent
	\brief Event for if a mouse button has been pressed
	*/
	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {} //!< Constructor

		static EventType getStaticType() { return EventType::MouseButtonPressed; } //!< Returns the type even if the event has not been created
		EventType getEventType() const override { return EventType::MouseButtonPressed; } //!< Returns the event type
	};

	/**
	\class MouseButtonReleasedEvent
	\brief Event for if a mouse button has been released
	*/
	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {} //!< Constructor

		static EventType getStaticType() { return EventType::MouseButtonReleased; } //!< Returns the type even if the event has not been created
		EventType getEventType() const override { return EventType::MouseButtonReleased; } //!< Returns the event type
	};

	/**
	\class MouseMovedEvent
	\brief Event for if the mouse has been moved
	*/
	class MouseMovedEvent : public Event
	{
	private:
		float m_fMouseX; //!< The new mouse x position
		float m_fMouseY; //!< The new mouse y position
	public:
		MouseMovedEvent(float x, float y) : m_fMouseX(x), m_fMouseY(y) {} //!< Constructor

		static EventType getStaticType() { return EventType::MouseMoved; } //!< Returns the type even if the event has not been created
		EventType getEventType() const override { return EventType::MouseMoved; } //!< Returns the event type
		int getCategoryFlags() const override { return EventCategoryInput | EventCategoryMouse; } //!< Returns the category flags

		inline float getX() const { return m_fMouseX; } //!< Return the mouse x position
		inline float getY() const { return m_fMouseY; } //!< Return the mouse y position
	};

	/**
	\class MouseScrolledEvent
	\brief Event for if the user scrolls with the mouse
	*/
	class MouseScrolledEvent : public Event
	{
	private:
		float m_fXOffset; //!< The x offset
		float m_fYOffset; //!< The y offset
	public:
		MouseScrolledEvent(float x, float y) : m_fXOffset(x), m_fYOffset(y) {} //!< Constructor

		static EventType getStaticType() { return EventType::MouseScrolled; } //!< Returns the type even if the event has not been created
		EventType getEventType() const override { return EventType::MouseScrolled; } //!< Returns the event type
		int getCategoryFlags() const override { return EventCategoryInput | EventCategoryMouse; } //!< Returns the category flags

		inline float getXOffset() const { return m_fXOffset; } //!< Return the x offset
		inline float getYOffset() const { return m_fYOffset; } //!< Return the y offset
	};
}