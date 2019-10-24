/** \file window.h
*/
#pragma once

#include "graphicsContext.h"

#include "events/windowEvents.h"
#include "events/keyEvents.h"
#include "events/mouseEvents.h"

#include <string>
#include <functional>

namespace Engine
{
	//! Contains the properties of the window
	struct WindowProperties
	{
		std::string m_title; //!< The title of the window
		unsigned int m_width; //!< The width of the window in pixels
		unsigned int m_height; //!< The height of the window in pixels
		bool m_isFullScreen; //!< Is the window fullscreen
		bool m_isVSync; //!< Is vSync on

		float m_fAspect; //!< Aspect ratio for the viewport

		unsigned int m_storedWidth; //!< Width that the window goes back to if going to fullscreen and back
		unsigned int m_storedHeight; //!< Height that the window goes back to if going to fullscreen and back
		int m_storedXPos; //!< X position that the window goes back to if going to fullscreen and back
		int m_storedYPos; //!< Y position that the window goes back to if going to fullscreen and back

		//! Set all of the properties to default values if the user doesn't set them when creating the window
		WindowProperties(const std::string& title = "My Window", unsigned int width = 800, unsigned int height = 600, bool fullscreen = false) : 
			m_title(title), m_width(width), m_height(height), m_isFullScreen(fullscreen), m_fAspect((float)width / (float)height), m_storedWidth(width), m_storedHeight(height) {}
	};

	/**
	\class Window
	\brief A base (interface) class for any window to use

	Abstract class with functions to be overridden by classes which inherit from it.
	*/
	class Window
	{
	protected:
		std::shared_ptr<GraphicsContext> m_context; //!< Pointer to the GraphicsContext class
	public:
		//! Initialize the window, called when making a new window
		/*!
		\param properties All of the properties given to the window
		*/
		virtual void init(const WindowProperties& properties) = 0;
		//! Called when closing the window
		virtual void close() = 0;
		//! Destructor
		virtual ~Window() {};
		//! Called every frame to update the window
		/*!
		\param timestep The time passed after previous onUpdate function call
		*/
		virtual void onUpdate(float timestep) = 0;
		//! Called when the window size is changed
		/*!
		\param width The new width of the window
		\param height The new height of the window
		*/
		virtual void onResize(unsigned int width, unsigned int height) = 0;
		//! Called to enter or exit fullscreen mode
		/*!
		\param fullscreen Whether the window is being made fullscreen or not
		*/
		virtual void setFullscreen(bool fullscreen) = 0;
		//! Set if vSync is on
		/*!
		\param VSync True if turning vSync on
		*/
		virtual void setVSync(bool VSync) = 0;
		//! Set the event callback for the window
		/*!
		\param callback Callback for events
		*/
		virtual void setEventCallback(const std::function<void(Event&)>& callback) = 0;
		//! Function to get the width of the window
		/*!
		\return The width of the window in pixels
		*/
		virtual unsigned int getWidth() const = 0;
		//! Function to get the height of the window
		/*!
		\return The height of the window in pixels
		*/
		virtual unsigned int getHeight() const = 0;
		//! Function to get a pointer to the window
		/*!
		\return The stored pointer to the native window, the pointer is void so that it can return a pointer to any type of window
		*/
		virtual void* getNativeWindow() const = 0;
		//! Function to check if the window is fullscreen
		/*!
		\return A bool for if the window is fullscreen
		*/
		virtual bool isFullScreenMode() const = 0;
		//! Function to check if vSync is on
		/*!
		\return A bool for if vSync is on
		*/
		virtual bool isVSync() const = 0;

		//! Create a window and return a pointer to it
		/*!
		\param properties The properties given to the window, set to default values if none are provided
		\return A pointer to the new window
		*/
		static Window* create(const WindowProperties& properties = WindowProperties());
	};
}

