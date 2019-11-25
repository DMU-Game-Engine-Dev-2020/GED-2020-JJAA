/** \file renderCommand.h
*/
#pragma once

namespace Engine
{
	/**
	\class RenderCommand
	\brief Base class for any render commands
	*/
	class RenderCommand
	{
	public:
		virtual void action() = 0; //!< Action the command

		//! Function to create a command to clear the colour buffer and the depth buffer
		/*!
		\return A pointer to the command
		*/
		static RenderCommand* clearDepthColourBufferCommand();
		//! Function to create a command to set the colour of the window
		/*!
		\param r New value for the red channel
		\param g New value for the green channel
		\param b New value for the blue channel
		\param a New value for the alpha channel
		\return A pointer to the command
		*/
		static RenderCommand* setClearColourCommand(float r, float g, float b, float a);
		//! Function to create a command to set the depth test value to less
		/*!
		\param enabled If turning on or off
		\return A pointer to the command
		*/
		static RenderCommand* setDepthTestLessCommand(bool enabled);
		//! Function to create a command to turn backface culling on or off
		/*!
		\param enabled If turning on or off
		\return A pointer to the command
		*/
		static RenderCommand* setBackfaceCullingCommand(bool enabled);

		static RenderCommand* clearColourBufferCommand();

		static RenderCommand* setOneMinusAlphaBlendingCommand(bool enabled);
	};
}