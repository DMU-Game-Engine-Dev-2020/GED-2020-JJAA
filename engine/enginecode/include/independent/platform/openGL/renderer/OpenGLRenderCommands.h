#pragma once

#include "rendering/renderer/renderCommand.h"

namespace Engine
{
	class OpenGLClearDepthColourBufferCommand : public RenderCommand
	{
	public:
		OpenGLClearDepthColourBufferCommand() {}
		void action() override;
	};


	class OpenGLSetClearColourCommand : public RenderCommand
	{
	private:
		float m_r, m_g, m_b, m_a;
	public:
		OpenGLSetClearColourCommand(float r, float g, float b, float a) : m_r(r), m_g(g), m_b(b), m_a(a) {}
		void action() override;
	};


	class OpenGLSetDepthTestLessCommand : public RenderCommand
	{
	private:
		bool m_bEnabled;
	public:
		OpenGLSetDepthTestLessCommand(bool enabled) : m_bEnabled(enabled) {}
		void action() override;
	};


	class OpenGLSetBackfaceCullingCommand : public RenderCommand
	{
	private:
		bool m_bEnabled;
	public:
		OpenGLSetBackfaceCullingCommand(bool enabled) : m_bEnabled(enabled) {}
		void action() override;
	};
}