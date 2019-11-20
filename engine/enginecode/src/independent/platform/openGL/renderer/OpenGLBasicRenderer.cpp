#include "engine_pch.h"

#include "platform/openGL/renderer/OpenGLBasicRenderer.h"

#include <glad/glad.h>

namespace Engine
{
	void OpenGLBasicRenderer::actionCommand(RenderCommand* command)
	{
		command->action();
		delete command;
	}

	void OpenGLBasicRenderer::beginScene(const SceneData& sceneData)
	{
		for (auto uboPair : sceneData)
		{
			unsigned int offset = 0;
			//unsigned int size;
			int i = 0;

			UniformBufferLayout layout = uboPair.first->getLayout();

			for (auto bufferElement : layout)
			{
				uboPair.first->setData(bufferElement.m_iOffset, bufferElement.m_iSize, uboPair.second[i]);
				i++;
			}
		}
	}

	void OpenGLBasicRenderer::submit(const std::shared_ptr<Material>& material)
	{
		auto shader = material->getShader();
		shader->bind();

		auto geometry = std::get<std::shared_ptr<VertexArray>>(material->getGeometry());
		geometry->bind();

		auto perDrawData = material->getData();
		for (auto dataPair : perDrawData)
		{
			shader->uploadData(dataPair.first, dataPair.second);
		}

		glDrawElements(GL_TRIANGLES, geometry->getDrawCount(), GL_UNSIGNED_INT, nullptr);
	}
}