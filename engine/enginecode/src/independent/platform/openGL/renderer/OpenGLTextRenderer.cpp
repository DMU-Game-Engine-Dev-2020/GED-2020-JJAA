#include "engine_pch.h"

#include "platform/openGL/renderer/OpenGLTextRenderer.h"

#include <glad/glad.h>

namespace Engine
{
	void OpenGLTextRenderer::actionCommand(RenderCommand* command)
	{
		command->action(); // Perform the command
		delete command; // Delete the command
	}

	void OpenGLTextRenderer::submit(const std::shared_ptr<Material>& material)
	{
		auto shader = material->getShader();
		shader->bind(); // Bind the shader

		// Get the geometry from the material
		auto geometry = std::get<std::shared_ptr<VertexArray>>(material->getGeometry());
		geometry->bind(); // Bind the geometry

		auto perDrawData = material->getData(); // Get the data to be uploaded
		// For each pair with uniform data
		for (auto dataPair : perDrawData)
		{
			// Call the function to upload the data
			shader->uploadData(dataPair.first, dataPair.second);
		}
		// Draw the material
		glDrawElements(GL_QUADS, geometry->getDrawCount(), GL_UNSIGNED_INT, nullptr);
	}
}