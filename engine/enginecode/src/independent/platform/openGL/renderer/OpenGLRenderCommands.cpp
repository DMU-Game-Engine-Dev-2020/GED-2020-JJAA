#include "engine_pch.h"

#include "platform/openGL/renderer/OpenGLRenderCommands.h"

#include <glad/glad.h>

namespace Engine
{
	void OpenGLClearDepthColourBufferCommand::action()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLSetClearColourCommand::action()
	{
		glClearColor(m_r, m_g, m_b, m_a);
	}

	void OpenGLSetDepthTestLessCommand::action()
	{
		if (m_bEnabled)
		{
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}
	}

	void OpenGLSetBackfaceCullingCommand::action()
	{
		if (m_bEnabled)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}
	}
}