#pragma once

#include "systems/resourceManager.h"
#include "rendering/renderer/renderer.h"
#include "cameras/cameraController.h"

namespace Engine
{
	class Layer
	{
	protected:
		std::string m_name;
		std::shared_ptr<ResourceManager> m_pResources;
		std::shared_ptr<Renderer> m_pRenderer;
		std::shared_ptr<CameraController> m_pCamera;
		std::shared_ptr<UniformBuffer> m_pUBO;

		SceneData m_sceneData;
	public:
		Layer(const std::string& layer) : m_name(layer) {}

		virtual void onAttach() = 0;
		virtual void onDetach() = 0;
		virtual void onUpdate(float timestep) = 0;
		virtual void onEvent(Event& e) = 0;

		inline const std::string& getName() const { return m_name; }
	};
}