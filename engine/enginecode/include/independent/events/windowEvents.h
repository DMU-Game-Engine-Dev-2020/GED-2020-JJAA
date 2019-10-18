/** \file windowEvents.h
\brief Contains classes used for window events
*/

#pragma once

#include "events/event.h"

namespace Engine
{
	/**
	\class WindowCloseEvent
	\brief Event for if the window has been closed
	*/
	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {} //!< Constructor

		static EventType getStaticType() { return EventType::WindowClose; } //!< Returns the type even if the event has not been created
		EventType getEventType() const override { return EventType::WindowClose; } //!< Returns the event type
		int getCategoryFlags() const override { return EventCategoryWindow; } //!< Returns the category flags
	};

	/**
	\class WindowResizeEvent
	\brief Event for if the size of the window has been changed
	*/
	class WindowResizeEvent : public Event
	{
	private:
		int m_iWidth; //!< Width of the window
		int m_iHeight; //!< Height of the window
	public:
		WindowResizeEvent(int width, int height) : m_iWidth(width), m_iHeight(height) {} //!< Constructor

		static EventType getStaticType() { return EventType::WindowResize; } //!< Returns the type even if the event has not been created
		EventType getEventType() const override { return EventType::WindowResize; }	//!< Returns the event type
		int getCategoryFlags() const override { return EventCategoryWindow; } //!< Returns the category flags

		inline int getWidth() const { return m_iWidth; } //!< Return the width of the window
		inline int getHeight() const { return m_iHeight; } //!< Return the height of the window
	};

	/**
	\class WindowFocusEvent
	\brief Event for if the window is being focused on 
	*/
	class WindowFocusEvent : public Event
	{
	private:
		float m_fXPos;
		float m_fYPos;
	public:
		WindowFocusEvent(float xPos, float yPos) : m_fXPos(xPos), m_fYPos(yPos) {} //!< Constructor

		static EventType getStaticType() { return EventType::WindowFocus; } //!< Returns the type even if the event has not been created
		EventType getEventType() const override { return EventType::WindowFocus; } //!< Returns the event type
		int getCategoryFlags() const override { return EventCategoryWindow; } //!< Returns the category flags

		float getXPos() { return m_fXPos; } //!< Returns the x position
		float getYPos() { return m_fYPos; } //!< Returns the y position
	};

	/**
	\class WindowLostFocusEvent
	\brief Event for if the window is not being focused on
	*/
	class WindowLostFocusEvent : public Event
	{
	private:
		float m_fXPos;
		float m_fYPos;
	public:
		WindowLostFocusEvent(float xPos, float yPos) : m_fXPos(xPos), m_fYPos(yPos) {} //!< Constructor

		static EventType getStaticType() { return EventType::WindowLostFocus; } //!< Returns the type even if the event has not been created
		EventType getEventType() const override { return EventType::WindowLostFocus; } //!< Returns the event type
		int getCategoryFlags() const override { return EventCategoryWindow; } //!< Returns the category flags

		float getXPos() { return m_fXPos; } //!< Returns the x position
		float getYPos() { return m_fYPos; } //!< Returns the y position
	};

	/**
	\class WindowMovedEvent
	\brief Event for if the window is being moved
	*/
	class WindowMovedEvent : public Event
	{
	public:
		WindowMovedEvent() {} //!< Constructor

		static EventType getStaticType() { return EventType::WindowMoved; } //!< Returns the type even if the event has not been created
		EventType getEventType() const override { return EventType::WindowMoved; } //!< Returns the event type
		int getCategoryFlags() const override { return EventCategoryWindow; } //!< Returns the category flags
	};
}