#pragma once

namespace Engine
{
	class RenderCommand
	{
	public:
		virtual void action() = 0;

		static RenderCommand* clearDepthColourBufferCommand();
		static RenderCommand* setClearColourCommand(float r, float g, float b, float a);
		static RenderCommand* setDepthTestLessCommand(bool enabled);
		static RenderCommand* setBackfaceCullingCommand(bool enabled);
	};
}