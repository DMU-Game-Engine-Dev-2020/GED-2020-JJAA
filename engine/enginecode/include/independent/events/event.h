/** \file event.h
\brief Contains classes and enums used for events
*/

#pragma once

namespace Engine
{
	/**
	\class EventType
	\brief Enum for all the types of event
	*/
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory //! Categories for events. Bit shifting to give each category a bit pattern
	{
		None = 0,                          // Bit patterns
		EventCategoryWindow = 1 << 0,      // 00000001
		EventCategoryInput = 1 << 1,       // 00000010
		EventCategoryKeyboard = 1 << 2,    // 00000100
		EventCategoryMouse = 1 << 3,       // 00001000
		EventCategoryMouseButton = 1 << 4  // 00010000
	};

	/**
	\class Event
	\brief Base class for all events.

	All events will inherit from this class, an abstract class with pure virtual functions which have to be implemented
	in the classes which inherit from it
	*/
	class Event
	{
	private:
		bool m_bHandled = false; //!< If the event has been handled
	public:
		virtual EventType getEventType() const = 0; //!< To be overridden, will return the event type
		virtual int getCategoryFlags() const = 0; //!< To be overridden, will return the event category
		inline bool handled() const { return m_bHandled; } //!< Returns if the event has been handled or not
		inline void handle(bool isHandled) { m_bHandled = isHandled; } //!< Set if the event has been handled
		inline bool isInCategory(EventCategory category) { return getCategoryFlags()& category; } //!< Check if the event is in a category
	};

	/**
	\class EventDispatcher
	\brief Dispatches events to boolean functions which take a reference to the event
	*/
	class EventDispatcher
	{
		template<typename T>
		using EventFunc = std::function<bool(T&)>; //!< EventFunc is a function which returns a boolean and takes a reference to template type T
	private:
		Event& m_event; //!< Reference to the Event
	public:
		EventDispatcher(Event& event) : m_event(event) {} //!< Constructor, set the stored event reference to the one passed in

		//! Function comparing the type of the stored event with the type of the event passed in
		template<typename T>
		bool dispatch(EventFunc<T> func)
		{
			// If the event types match
			if (m_event.getEventType() == T::getStaticType())
			{
				// Handle the event
				m_event.handle(func(*((T*)&m_event)));
				return true;
			}
			// The event types didn't match
			return false;
		}
	};
}