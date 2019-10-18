#include "engine_pch.h"
#include "windows/glfwWindow.h"

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
	}

	void GLFWWindowImpl::close()
	{
		glfwDestroyWindow(m_pNativeWindow);
	}

	void GLFWWindowImpl::onUpdate(float timestep)
	{
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