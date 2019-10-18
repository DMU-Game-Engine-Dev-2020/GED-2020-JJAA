#include "engine_pch.h"
#include "windows/GLFW/glfwWindow.h"

namespace Engine
{
	static bool s_bGLFWInitialized = false;

	Window* Window::create(const WindowProperties& properties)
	{
		return new GLFWWindowImpl(properties);
	}

	GLFWWindowImpl::GLFWWindowImpl(const WindowProperties& properties)
	{
		init(properties);
	}

	GLFWWindowImpl::~GLFWWindowImpl()
	{
		close();
	}

	void GLFWWindowImpl::init(const WindowProperties& properties)
	{
		m_properties.m_title = properties.m_title;
		m_properties.m_width = properties.m_width;
		m_properties.m_height = properties.m_height;

		LOG_INFO("Creating window {0} ({1}, {2})", properties.m_title, properties.m_width, properties.m_height);

		if (!s_bGLFWInitialized)
		{
			glfwInit();
			s_bGLFWInitialized = true;
		}

		m_pNativeWindow = glfwCreateWindow((int)properties.m_width, (int)properties.m_height, m_properties.m_title.c_str(), nullptr, nullptr);

		m_context = std::shared_ptr<GraphicsContext>(new OpenGL_GLFWGraphicsContext(m_pNativeWindow));
		m_context->init();
		glfwSetWindowUserPointer(m_pNativeWindow, &m_callback);
		setVSync(true);

		glfwSetWindowCloseCallback(m_pNativeWindow,
			[](GLFWwindow* window)
		{
			std::function<void(Event&)>& callback = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			callback(event);
		});

		glfwSetWindowSizeCallback(m_pNativeWindow,
			[](GLFWwindow* window, int width, int height)
		{
			std::function<void(Event&)>& callback = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(window);

			WindowResizeEvent event(width, height);
			callback(event);
		});

		glfwSetWindowFocusCallback(m_pNativeWindow,
			[](GLFWwindow* window, int focused)
		{
			std::function<void(Event&)>& callback = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(window);

			if (focused == GLFW_TRUE)
			{
				WindowFocusEvent event(0, 0);
				callback(event);
			}
			else if (focused == GLFW_FALSE)
			{
				WindowLostFocusEvent event(0, 0);
				callback(event);
			}
		});

		glfwSetKeyCallback(m_pNativeWindow,
			[](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			std::function<void(Event&)>& callback = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(window);

			if (action == GLFW_PRESS)
			{
				KeyPressedEvent event(key, 0);
				callback(event);
			}
			else if (action == GLFW_RELEASE)
			{
				KeyReleasedEvent event(key);
				callback(event);
			}
			else if (action == GLFW_REPEAT)
			{
				KeyPressedEvent event(key, 1);
				callback(event);
			}
		});

		glfwSetMouseButtonCallback(m_pNativeWindow,
			[](GLFWwindow* window, int button, int action, int mods)
		{
			std::function<void(Event&)>& callback = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(window);

			if (action == GLFW_PRESS)
			{
				MouseButtonPressedEvent event(button);
				callback(event);
			}
			else if (action == GLFW_RELEASE)
			{
				MouseButtonReleasedEvent event(button);
				callback(event);
			}
		});

		glfwSetCursorPosCallback(m_pNativeWindow,
			[](GLFWwindow* window, double xPos, double yPos)
		{
			std::function<void(Event&)>& callback = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			callback(event);
		});

		glfwSetScrollCallback(m_pNativeWindow,
			[](GLFWwindow* window, double xOffset, double yOffset)
		{
			std::function<void(Event&)>& callback = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			callback(event);
		});
	}

	void GLFWWindowImpl::close()
	{
		glfwDestroyWindow(m_pNativeWindow);
	}

	void GLFWWindowImpl::onUpdate(float timestep)
	{
		glClearColor(1, 0, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwPollEvents();
		m_context->swapBuffers();
	}

	void GLFWWindowImpl::onResize(unsigned int width, unsigned int height)
	{
		m_properties.m_width = width;
		m_properties.m_height = height;
	}

	void GLFWWindowImpl::setVSync(bool vSync)
	{
		if (vSync)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_properties.m_isVSync = vSync;
	}

	void GLFWWindowImpl::setEventCallback(const std::function<void(Event&)>& callback)
	{
		m_callback = callback;
	}
}