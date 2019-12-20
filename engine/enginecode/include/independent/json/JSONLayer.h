#pragma once

#include "core/layer.h"

#include "components/CGO.h"
#include "components/materialComponent.h"
#include "components/positionComponent.h"
#include "components/velocityComponent.h"
#include "components/oscilateComponent.h"
#include "components/rotateComponent.h"
#include "components/textureComponent.h"
#include "components/textureSwitchComponent.h"

namespace Engine 
{
	class JSONLayer : public Layer
	{
	protected:
		std::vector<std::shared_ptr<Engine::GameObject>> m_gameObjects;			//!< Game objects which belong to the layer
		std::vector<std::shared_ptr<Engine::MaterialComponent>> m_materials;	//!< Material components
		std::vector<std::shared_ptr<Engine::PositionComponent>> m_positions;	//!< Position components
		std::vector<std::shared_ptr<Engine::VelocityComponent>> m_velocities;	//!< Velocity components
		std::vector<std::shared_ptr<Engine::TextureComponent>> m_textures;	    //!< Texture components
		std::vector<std::shared_ptr<Engine::ControllerComponent>> m_controllers;//!< Controler components

		std::vector<std::shared_ptr<Engine::RenderCommand>> m_initCommands;						//!< Render commands issued on renderer start
		std::vector<std::shared_ptr<Engine::RenderCommand>> m_predrawCommands;					//!< Render commands issued before materials are drawn
		std::vector<std::shared_ptr<Engine::RenderCommand>> m_postdrawCommands;					//!< Render commands issued after materials are drawn
		std::vector<std::shared_ptr<Engine::RenderCommand>> m_exitCommands;						//!< Render commands issued prior to renderer shutdown

		std::list<void*> m_data;			//!< Misc data
		std::string m_filepath;				//!< JSON filepath
	public:
		//! Constructor
		JSONLayer(const std::string& filepath, const std::string& name = "Layer") : Layer(name), m_filepath(filepath) {}	

		virtual void onAttach() override; 												//!< Runs when layer is attached
		virtual void onDetach() override;												//!< Runs when layer is detached
		virtual void onUpdate(float timestep) override;									//!< Runs every frame
		virtual void onEvent(Event& event) override;									//!< Runs when an event occurs


		inline std::vector<std::shared_ptr<Engine::GameObject>>& getGameObjects() { return m_gameObjects; }
		inline std::vector<std::shared_ptr<Engine::MaterialComponent>>& getMaterials() { return m_materials; }
		inline std::vector<std::shared_ptr<Engine::PositionComponent>>& getPositions() { return m_positions; }
		inline std::vector<std::shared_ptr<Engine::VelocityComponent>>& getVelocities() { return m_velocities; }
		inline std::vector<std::shared_ptr<Engine::TextureComponent>>& getTextures() { return m_textures; }
		inline std::vector<std::shared_ptr<Engine::ControllerComponent>>& getControllers() { return m_controllers; }

		inline std::vector<std::shared_ptr<Engine::RenderCommand>>& getPredrawCommands() { return m_predrawCommands; }
		inline std::vector<std::shared_ptr<Engine::RenderCommand>>& getPostdrawCommands() { return m_postdrawCommands; }
		inline std::vector<std::shared_ptr<Engine::RenderCommand>>& getInitCommands() { return m_initCommands; }
		inline std::vector<std::shared_ptr<Engine::RenderCommand>>& getExitCommands() { return m_exitCommands; }

		inline std::list<void*>& getData() { return m_data; }
	};
}