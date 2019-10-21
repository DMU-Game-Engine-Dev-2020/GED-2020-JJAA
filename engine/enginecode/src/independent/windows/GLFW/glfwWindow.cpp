/** \file glfwWindow.cpp
*/
#include "engine_pch.h"
#include "windows/GLFW/glfwWindow.h"
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
			ENGINE_ASSERT(success--, "Could not initialise GLFW");

			s_bGLFWInitialized = true; // So that if another window is created GLFW is not re initialized
		}
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

	void GLFWWindowImpl::onUpdate(float timestep)
	{
		glClearColor(1, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwPollEvents();
		m_context->swapBuffers();
	}

	void GLFWWindowImpl::onResize(unsigned int width, unsigned int height)
	{
		// Set the stored size to the new size
		m_properties.m_width = width;
		m_properties.m_height = height;
	}

	void GLFWWindowImpl::setFullscreen(bool fullscreen)
	{
		m_properties.m_isFullScreen = fullscreen;

		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);

		if (fullscreen)
		{
			glfwSetWindowMonitor(m_pNativeWindow, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
		}
		else
		{
			glfwSetWindowMonitor(m_pNativeWindow, nullptr, m_properties.m_width / 4, m_properties.m_height / 4, m_properties.m_startWidth, m_properties.m_startHeight, mode->refreshRate);
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