#include "engine_pch.h"

#include "platform/openGL/renderer/OpenGLSuperSimpleRenderer.h"

#include <glad/glad.h>

namespace Engine
{
	void OpenGLSuperSimpleRenderer::actionCommand(RenderCommand* command)
	{
		command->action();
		delete command;
	}

	void OpenGLSuperSimpleRenderer::submit(const std::shared_ptr<Material>& material)
	{
		auto shader = material->getShader();
		shader->bind();

		auto geometry = std::get<std::shared_ptr<VertexArray>>(material->getGeometry());
		geometry->bind();

		auto uniformData = material->getData();
		for (auto dataPair : uniformData)
		{
			shader->uploadData(dataPair.first, dataPair.second);
		}

		glDrawElements(GL_TRIANGLES, geometry->getDrawCount(), GL_UNSIGNED_INT, nullptr);
	}
}