/** \file JSONLayer.h
*/
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
#include "components/textComponent.h"

namespace Engine 
{
	/**
	\class JSONLayer
	\brief Class that creates a layer from a loaded JSON file
	*/
	class JSONLayer : public Layer
	{
	protected:
		std::vector<std::shared_ptr<Engine::GameObject>> m_gameObjects;			//!< Game objects which belong to the layer
		std::vector<std::shared_ptr<Engine::MaterialComponent>> m_materials;	//!< Material components
		std::vector<std::shared_ptr<Engine::TextComponent>> m_texts;            //!< Text components
		std::vector<std::shared_ptr<Engine::PositionComponent>> m_positions;	//!< Position components
		std::vector<std::shared_ptr<Engine::VelocityComponent>> m_velocities;	//!< Velocity components
		std::vector<std::shared_ptr<Engine::TextureComponent>> m_textures;	    //!< Texture components
		std::vector<std::shared_ptr<Engine::ControllerComponent>> m_controllers;//!< Controler components

		std::vector<std::shared_ptr<Engine::RenderCommand>> m_initCommands;		//!< Render commands issued on renderer start
		std::vector<std::shared_ptr<Engine::RenderCommand>> m_predrawCommands;	//!< Render commands issued before materials are drawn
		std::vector<std::shared_ptr<Engine::RenderCommand>> m_postdrawCommands;	//!< Render commands issued after materials are drawn
		std::vector<std::shared_ptr<Engine::RenderCommand>> m_exitCommands;		//!< Render commands issued prior to renderer shutdown

		std::list<void*> m_data; //!< Misc data
		std::string m_filepath;	 //!< JSON filepath
	public:
		//! Constructor
		/*!
		\param filepath The location of the JSON file
		\param name The name of the layer
		*/
		JSONLayer(const std::string& filepath, const std::string& name = "Layer") : Layer(name), m_filepath(filepath) {}	

		//! Runs when layer is attached
		virtual void onAttach() override; 		
		//! Runs when layer is detached
		virtual void onDetach() override;	
		//! Runs every frame
		/*!
		\param timestep The time since the previous frame
		*/
		virtual void onUpdate(float timestep) override;	
		//! Runs when an event occurs
		/*!
		\param event The event that has occurred
		*/
		virtual void onEvent(Event& event) override;	

		//! Function to get the gameobjects in the layer
		/*!
		\return A vector containing shared pointers to all of the gameobjects
		*/
		inline std::vector<std::shared_ptr<Engine::GameObject>>& getGameObjects() { return m_gameObjects; }
		//! Function to get the material components in the layer
		/*!
		\return A vector containing shared pointers to all of the material components
		*/
		inline std::vector<std::shared_ptr<Engine::MaterialComponent>>& getMaterials() { return m_materials; }
		//! Function to get the text components in the layer
		/*!
		\return A vector containing shared pointers to all of the text components
		*/
		inline std::vector<std::shared_ptr<Engine::TextComponent>>& getTexts() { return m_texts; }
		//! Function to get the position components in the layer
		/*!
		\return A vector containing shared pointers to all of the position components
		*/
		inline std::vector<std::shared_ptr<Engine::PositionComponent>>& getPositions() { return m_positions; }
		//! Function to get the velocity components in the layer
		/*!
		\return A vector containing shared pointers to all of the velocity components
		*/
		inline std::vector<std::shared_ptr<Engine::VelocityComponent>>& getVelocities() { return m_velocities; }
		//! Function to get the texture components in the layer
		/*!
		\return A vector containing shared pointers to all of the texture components
		*/
		inline std::vector<std::shared_ptr<Engine::TextureComponent>>& getTextures() { return m_textures; }
		//! Function to get the controller components in the layer
		/*!
		\return A vector containing shared pointers to all of the controller components
		*/
		inline std::vector<std::shared_ptr<Engine::ControllerComponent>>& getControllers() { return m_controllers; }

		//! Function to get the predraw render commands in the layer
		/*!
		\return A vector containing shared pointers to all of the predraw render commands
		*/
		inline std::vector<std::shared_ptr<Engine::RenderCommand>>& getPredrawCommands() { return m_predrawCommands; }
		//! Function to get the postdraw render commands in the layer
		/*!
		\return A vector containing shared pointers to all of the postdraw render commands
		*/
		inline std::vector<std::shared_ptr<Engine::RenderCommand>>& getPostdrawCommands() { return m_postdrawCommands; }
		//! Function to get the initialization render commands in the layer
		/*!
		\return A vector containing shared pointers to all of the initialization render commands
		*/
		inline std::vector<std::shared_ptr<Engine::RenderCommand>>& getInitCommands() { return m_initCommands; }
		//! Function to get the exit render commands in the layer
		/*!
		\return A vector containing shared pointers to all of the exit render commands
		*/
		inline std::vector<std::shared_ptr<Engine::RenderCommand>>& getExitCommands() { return m_exitCommands; }

		//! Function to get the misc data of the layer
		/*!
		\return A list of void pointers to data
		*/
		inline std::list<void*>& getData() { return m_data; }
	};
}