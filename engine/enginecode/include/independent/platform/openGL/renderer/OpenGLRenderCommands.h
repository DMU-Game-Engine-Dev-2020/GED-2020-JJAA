/** \file OpenGLRenderCommands.h
\brief File containing class declerations for the open GL render commands
*/
#pragma once

#include "rendering/renderer/renderCommand.h"

namespace Engine
{
	/**
	\class OpenGLClearDepthColourBufferCommand
	\brief Open GL command to clear the colour buffer and the depth buffer
	*/
	class OpenGLClearDepthColourBufferCommand : public RenderCommand
	{
	public:
		//! Constructor
		OpenGLClearDepthColourBufferCommand() {}
		void action() override; //!< Action the command
	};

	/**
	\class OpenGLSetClearColourCommand
	\brief Open GL command to set the colour of the window
	*/
	class OpenGLSetClearColourCommand : public RenderCommand
	{
	private:
		float m_r; //!< Red channel value
		float m_g; //!< Green channel value
		float m_b; //!< Blue channel value
		float m_a; //!< Alpha channel value
	public:
		//! Constructor
		/*!
		\param r New value for the red channel
		\param g New value for the green channel
		\param b New value for the blue channel
		\param a New value for the alpha channel
		*/
		OpenGLSetClearColourCommand(float r, float g, float b, float a) : m_r(r), m_g(g), m_b(b), m_a(a) {}
		void action() override; //!< Action the command
	};

	/**
	\class OpenGLSetDepthTestLessCommand
	\brief Open GL command to set the depth test value to less
	*/
	class OpenGLSetDepthTestLessCommand : public RenderCommand
	{
	private:
		bool m_bEnabled; //!< If turning on or off
	public:
		//! Constructor
		/*!
		\param enabled If turning on or off
		*/
		OpenGLSetDepthTestLessCommand(bool enabled) : m_bEnabled(enabled) {}
		void action() override; //!< Action the command
	};

	/**
	\class OpenGLSetBackfaceCullingCommand
	\brief Open GL command to turn backface culling on or off
	*/
	class OpenGLSetBackfaceCullingCommand : public RenderCommand
	{
	private:
		bool m_bEnabled; //!< If turning on or off
	public:
		//! Constructor
		/*!
		\param enabled If turning on or off
		*/
		OpenGLSetBackfaceCullingCommand(bool enabled) : m_bEnabled(enabled) {}
		void action() override; //!< Action the command
	};
}