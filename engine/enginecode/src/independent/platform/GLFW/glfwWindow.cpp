/** \file glfwWindow.cpp
*/
#include "engine_pch.h"
#include "platform/GLFW/glfwWindow.h"
#include "core/core.h"

namespace Engine
{
	static bool s_bGLFWInitialized = false; // If GLFW is alreasy initialized

	Window* Window::create(const WindowProperties& properties)
	{
		return new GLFWWindowImpl(properties); // Return a pointer to a GLFW window
	}

	GLFWWindowImpl::GLFWWindowImpl(const WindowProperties& properties)
	{
		// Call function init to set up the window
		init(properties);
	}

	GLFWWindowImpl::~GLFWWindowImpl()
	{
		// Call function close to close the window
		close();
	}

	void GLFWWindowImpl::init(const WindowProperties& properties)
	{
		// Set the properties to the ones passed in
		m_properties.m_title = properties.m_title;
		m_properties.m_width = properties.m_width;
		m_properties.m_height = properties.m_height;

		// Show in the log that a window is being created
		LOG_INFO("Creating window {0} ({1}, {2})", properties.m_title, properties.m_width, properties.m_height);

		// If GLFW has not been initialize
		if (!s_bGLFWInitialized)
		{
			int success = glfwInit(); // Initialize GLFW
			ENGINE_ASSERT(success, "Could not initialise GLFW");

			s_bGLFWInitialized = true; // So that if another window is created GLFW is not re initialized
		}

		m_pMonitor = glfwGetPrimaryMonitor();

		// Create a new window
		m_pNativeWindow = glfwCreateWindow((int)properties.m_width, (int)properties.m_height, m_properties.m_title.c_str(), nullptr, nullptr);

		// Set graphics context to a new GLFW Graphics Context object
		m_context = std::shared_ptr<GraphicsContext>(new OpenGL_GLFWGraphicsContext(m_pNativeWindow));
		m_context->init(); // Initialize the graphics context object
		glfwSetWindowUserPointer(m_pNativeWindow, &m_callback); // Set the window user data
		setVSync(true); // Turn vSync on

		// Set callbacks for events
		glfwSetWindowCloseCallback(m_pNativeWindow,
			[](GLFWwindow* window)
		{
			std::function<void(Event&)>& callback = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event; // Create event type
			callback(event); // Callback (call Application::onEvent)
		});

		glfwSetWindowSizeCallback(m_pNativeWindow,
			[](GLFWwindow* window, int width, int height)
		{
			std::function<void(Event&)>& callback = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(window);

			WindowResizeEvent event(width, height); // Create event type
			callback(event); // Callback (call Application::onEvent)
		});

		glfwSetWindowFocusCallback(m_pNativeWindow,
			[](GLFWwindow* window, int focused)
		{
			std::function<void(Event&)>& callback = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(window);

			// If window is being focused
			if (focused == GLFW_TRUE)
			{
				WindowFocusEvent event(0, 0); // Create event type
				callback(event); // Callback (call Application::onEvent)
			}
			// If window is losing focus
			else if (focused == GLFW_FALSE)
			{
				WindowLostFocusEvent event(0, 0); // Create event type
				callback(event); // Callback (call Application::onEvent)
			}
		});

		glfwSetKeyCallback(m_pNativeWindow,
			[](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			std::function<void(Event&)>& callback = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(window);

			// If the key is being pressed
			if (action == GLFW_PRESS)
			{
				KeyPressedEvent event(key, 0); // Create event type
				callback(event); // Callback (call Application::onEvent)
			}
			// If the key is being released
			else if (action == GLFW_RELEASE)
			{
				KeyReleasedEvent event(key); // Create event type
				callback(event); // Callback (call Application::onEvent)
			}
			// If the key press is a repeat
			else if (action == GLFW_REPEAT)
			{
				KeyPressedEvent event(key, 1); // Create event type
				callback(event); // Callback (call Application::onEvent)
			}
		});

		glfwSetMouseButtonCallback(m_pNativeWindow,
			[](GLFWwindow* window, int button, int action, int mods)
		{
			std::function<void(Event&)>& callback = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(window);

			// If the button is being pressed
			if (action == GLFW_PRESS)
			{
				MouseButtonPressedEvent event(button); // Create event type
				callback(event); // Callback (call Application::onEvent)
			}
			// If the button is being released
			else if (action == GLFW_RELEASE)
			{
				MouseButtonReleasedEvent event(button); // Create event type
				callback(event); // Callback (call Application::onEvent)
			}
		});

		glfwSetCursorPosCallback(m_pNativeWindow,
			[](GLFWwindow* window, double xPos, double yPos)
		{
			std::function<void(Event&)>& callback = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos); // Create event type
			callback(event); // Callback (call Application::onEvent)
		});

		glfwSetScrollCallback(m_pNativeWindow,
			[](GLFWwindow* window, double xOffset, double yOffset)
		{
			std::function<void(Event&)>& callback = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset); // Create event type
			callback(event); // Callback (call Application::onEvent)
		});
	}

	void GLFWWindowImpl::close()
	{
		// Destroy the window
		glfwDestroyWindow(m_pNativeWindow);
	}

	void GLFWWindowImpl::onUpdate(float timestep) // Function, update the window with passed timestep (time passed)
	{
		glfwPollEvents(); // GLFW polls events (check current status)
		m_context->swapBuffers(); // Swap buffers for GLFW graphics context

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glClearColor(1, 0, 0, 1);

		int iWindowWidth; // The current width of the window in pixels
		int iWindowHeight; // The current height of the window in pixels

		int iViewportWidth; // The new width of the viewport
		int iViewportHeight; // The new height of the viewport

		int iViewportPositionX = 0; // The new x position of the viewport
		int iViewportPositionY = 0; // The new y position of the viewport

		// Get the size of the frame buffer
		glfwGetFramebufferSize(m_pNativeWindow, &iWindowWidth, &iWindowHeight);

		// Set the viewport to the current window size
		iViewportWidth = iWindowWidth;
		iViewportHeight = iWindowHeight;

		// If window is wider than the aspect ratio allows
		if (iWindowWidth > iWindowHeight * m_properties.m_fAspect) // Pillar box
		{
			// Calculate viewport width using current window height
			iViewportWidth = iWindowHeight * m_properties.m_fAspect;
			// Set x position so that the viewport is in the middle of the window
			iViewportPositionX = (iWindowWidth - iViewportWidth) / 2;
		}
		// If window is taller than the aspect ratio allows
		else if (iWindowHeight > iWindowWidth / m_properties.m_fAspect) // Letter box
		{
			// Calculate viewport height using current window width
			iViewportHeight = iWindowWidth / m_properties.m_fAspect;
			// Set y position so that the viewport is in the middle of the window
			iViewportPositionY = (iWindowHeight - iViewportHeight) / 2;
		}

		glViewport(iViewportPositionX, iViewportPositionY, iViewportWidth, iViewportHeight);

		glEnable(GL_SCISSOR_TEST);

		glClear(GL_COLOR_BUFFER_BIT);
		glScissor(iViewportPositionX, iViewportPositionY, iViewportWidth, iViewportHeight);

		glDisable(GL_SCISSOR_TEST);
	}

	void GLFWWindowImpl::onResize(unsigned int width, unsigned int height)
	{
		// Set the stored size to the new size
		m_properties.m_width = width;
		m_properties.m_height = height;
	}

	void GLFWWindowImpl::setFullscreen(bool fullscreen)
	{
		m_properties.m_isFullScreen = fullscreen; // Set the m_isFullScreen property
		// Get the current video mode of the monitor
		const GLFWvidmode* mode = glfwGetVideoMode(m_pMonitor);
		// If fullscreen is being turned on
		if (fullscreen)
		{
			// Set the current window size and position to the stored positions
			m_properties.m_storedWidth = m_properties.m_width;
			m_properties.m_storedHeight = m_properties.m_height;
			glfwGetWindowPos(m_pNativeWindow, &m_properties.m_storedXPos, &m_properties.m_storedYPos);
			// Makes the window fullscreen
			glfwSetWindowMonitor(m_pNativeWindow, m_pMonitor, 0, 0, mode->width, mode->height, mode->refreshRate);
		}
		else // If fullscreen is being turned off
		{
			// Set the window back to the stored size and position
			glfwSetWindowMonitor(m_pNativeWindow, nullptr, m_properties.m_storedXPos, m_properties.m_storedYPos, m_properties.m_storedWidth, m_properties.m_storedHeight, mode->refreshRate);
		}
	}

	void GLFWWindowImpl::setVSync(bool vSync)
	{
		// If turning on
		if (vSync)
			glfwSwapInterval(1); // Turn on
		// If turning off
		else
			glfwSwapInterval(0); // Turn off
		// Store if vSync is off or on
		m_properties.m_isVSync = vSync;
	}

	void GLFWWindowImpl::setEventCallback(const std::function<void(Event&)>& callback)
	{
		// Set the callback
		m_callback = callback;
	}
}