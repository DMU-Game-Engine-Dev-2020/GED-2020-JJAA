#pragma once

#include "rendering/renderer/renderer.h"

namespace Engine
{
	class OpenGLTextRenderer : public Renderer
	{
	public:
		void actionCommand(RenderCommand* command) override;
		void beginScene(const SceneData& sceneData) override;
		void endScene() override {}
		void submit(const std::shared_ptr<Material>& material) override;
		void flush() override {}
	};
}