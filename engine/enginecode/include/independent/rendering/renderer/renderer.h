#pragma once

#include "renderCommand.h"
#include "rendering/material.h"
#include "rendering/uniformBuffer.h"

#include <unordered_map>

namespace Engine
{
	using SceneData = std::unordered_map<std::shared_ptr<UniformBuffer>, std::vector<void*>>;

	class Renderer
	{
	public:
		virtual void actionCommand(RenderCommand* command) = 0;
		virtual void beginScene(const SceneData& sceneData) = 0;
		virtual void endScene() = 0;
		virtual void submit(const std::shared_ptr<Material>& material) = 0;
		virtual void flush() = 0;

		static Renderer* createSimple3D();
		static Renderer* createBasic3D();
	};
}