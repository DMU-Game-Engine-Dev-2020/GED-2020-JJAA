#pragma once

#include "windows/window.h"
#include "systems/log.h"
#include "windows/GLFW/openGL_GLFWGraphicsContext.h"

namespace Engine
{
	class GLFWWindowImpl : public Window
	{
	private:
		GLFWwindow* m_pNativeWindow;

		void init(const WindowProperties& properties) override;
		void close() override;
		WindowProperties m_properties;
		std::function<void(Event&)> m_callback;
		float m_fAspectRatio;
	public:
		GLFWWindowImpl(const WindowProperties& properties);
		~GLFWWindowImpl();
		void onUpdate(float timestep) override;
		void onResize(unsigned int width, unsigned int height) override;
		void setVSync(bool VSync) override;
		void setEventCallback(const std::function<void(Event&)>& callback) override;
		inline unsigned int getWidth() const override { return m_properties.m_width; }
		inline unsigned int getHeight() const override { return m_properties.m_height; }
		inline void* getNativeWindow() const override { return m_pNativeWindow; }
		inline bool isFullScreenMode() const override { return m_properties.m_isFullScreen; }
		inline bool isVSync() const override { return m_properties.m_isVSync; }
	};
}