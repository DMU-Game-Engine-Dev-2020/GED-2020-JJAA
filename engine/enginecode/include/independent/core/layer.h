/** \file layer.h
*/
#pragma once

#include "systems/resourceManager.h"
#include "rendering/renderer/renderer.h"
#include "cameras/cameraController.h"

namespace Engine
{
	/**
	\class Layer
	\brief Base class for layers to go in the layer stack
	*/
	class Layer
	{
	public:
		std::shared_ptr<Renderer> m_pRenderer; //!< Pointer to a renderer
		std::shared_ptr<CameraController> m_pCamera; //!< Pointer to a camera
	protected:
		std::string m_name; //!< The name of the layer
		std::shared_ptr<ResourceManager> m_pResources; //!< Pointer to the resource manager
		//std::shared_ptr<Renderer> m_pRenderer; //!< Pointer to a renderer
		//std::shared_ptr<CameraController> m_pCamera; //!< Pointer to a camera
		std::vector<std::shared_ptr<UniformBuffer>> m_UBOs; //!< Container of pointers to  uniform buffers

		SceneData m_sceneData; //!< Scene data for rendering
	public:
		//! Constructor
		/*!
		\param layer The name of the layer
		*/
		Layer(const std::string& layer) : m_name(layer) {}

		//! Function called when the layer is added
		virtual void onAttach() = 0;
		//! Function called when the layer is removed
		virtual void onDetach() = 0;
		//! Function to update the layer every frame
		/*!
		\param timestep The time since the previous frame
		*/
		virtual void onUpdate(float timestep) = 0;
		//! Function to handle events
		/*!
		\param e The event
		*/
		virtual void onEvent(Event& e) = 0;

		//! Function to get the name of the layer
		/*!
		\return The name of the layer
		*/
		inline const std::string& getName() const { return m_name; }

		//! Function to get the pointer to the camera controller
		/*!
		\return A pointer to the camera controller
		*/
		inline std::shared_ptr<CameraController> getCamera() { return m_pCamera; }
		//! Function to get the resourceManager
		/*!
		\return A pointer to the resource manager
		*/
		inline std::shared_ptr<ResourceManager> getResources() { return m_pResources; }
		//! Function to get the renderer
		/*!
		\return A pointer to the renderer
		*/
		inline std::shared_ptr<Renderer> getRenderer() { return m_pRenderer; }
		//! Function to get the scene data
		/*!
		\return A pointer to the scene data
		*/
		inline SceneData* getSceneData() { return &m_sceneData; }
	};
}