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
#include "components/playerComponent.h"

namespace Engine 
{
	/**
	\class JSONLayer
	\brief Class that creates a layer from a loaded JSON file
	*/
	class JSONLayer : public Layer
	{
	protected:
		std::vector<std::shared_ptr<GameObject>> m_gameObjects;			//!< Game objects which belong to the layer
		std::vector<std::shared_ptr<MaterialComponent>> m_materials;	//!< Material components
		std::vector<std::shared_ptr<TextComponent>> m_texts;            //!< Text components
		std::vector<std::shared_ptr<PositionComponent>> m_positions;	//!< Position components
		std::vector<std::shared_ptr<VelocityComponent>> m_velocities;	//!< Velocity components
		std::vector<std::shared_ptr<TextureComponent>> m_textures;	    //!< Texture components
		std::shared_ptr<PlayerComponent> m_player = nullptr;            //!< Player component
		std::vector<std::shared_ptr<ControllerComponent>> m_controllers;//!< Controler components

		std::vector<std::shared_ptr<RenderCommand>> m_initCommands;		//!< Render commands issued on renderer start
		std::vector<std::shared_ptr<RenderCommand>> m_predrawCommands;	//!< Render commands issued before materials are drawn
		std::vector<std::shared_ptr<RenderCommand>> m_postdrawCommands;	//!< Render commands issued after materials are drawn
		std::vector<std::shared_ptr<RenderCommand>> m_exitCommands;		//!< Render commands issued prior to renderer shutdown

		std::list<void*> m_data; //!< Misc data
		std::string m_filepath;	 //!< JSON filepath

		glm::mat4 m_camView; //!< The current cameras view
		glm::mat4 m_camProj; //!< The current cameras projection
		glm::mat4 m_camViewProj; //!< The current cameras view projecion
		glm::vec3 m_camPos; //!< The current cameras position
	public:
		//! Constructor
		/*!
		\param filepath The location of the JSON file
		\param name The name of the layer
		*/
		JSONLayer(const std::string& filepath, const std::string& name = "Layer") : Layer(name), m_filepath(filepath) {}	

		//! Runs when layer is attached
		void onAttach() override; 		
		//! Runs when layer is detached
		void onDetach() override;	
		//! Runs every frame
		/*!
		\param timestep The time since the previous frame
		*/
		void onUpdate(float timestep) override;	
		//! Runs when an event occurs
		/*!
		\param event The event that has occurred
		*/
		void onEvent(Event& event) override;
		//! Function to handle key press events
		/*!
		\param e The event
		\return If the event was successful or not
		*/
		bool onKeyPressed(KeyPressedEvent& e);

		//! Function to get the gameobjects in the layer
		/*!
		\return A vector containing shared pointers to all of the gameobjects
		*/
		inline std::vector<std::shared_ptr<GameObject>>& getGameObjects() { return m_gameObjects; }
		//! Function to get the material components in the layer
		/*!
		\return A vector containing shared pointers to all of the material components
		*/
		inline std::vector<std::shared_ptr<MaterialComponent>>& getMaterials() { return m_materials; }
		//! Function to get the text components in the layer
		/*!
		\return A vector containing shared pointers to all of the text components
		*/
		inline std::vector<std::shared_ptr<TextComponent>>& getTexts() { return m_texts; }
		//! Function to get the position components in the layer
		/*!
		\return A vector containing shared pointers to all of the position components
		*/
		inline std::vector<std::shared_ptr<PositionComponent>>& getPositions() { return m_positions; }
		//! Function to get the velocity components in the layer
		/*!
		\return A vector containing shared pointers to all of the velocity components
		*/
		inline std::vector<std::shared_ptr<VelocityComponent>>& getVelocities() { return m_velocities; }
		//! Function to get the texture components in the layer
		/*!
		\return A vector containing shared pointers to all of the texture components
		*/
		inline std::vector<std::shared_ptr<TextureComponent>>& getTextures() { return m_textures; }
		//! Function to get the player component in the layer
		/*!
		\return A shared pointer to the player component (if there is one)
		*/
		inline std::shared_ptr<PlayerComponent>& getPlayer() { return m_player; }
		//! Function to get the controller components in the layer
		/*!
		\return A vector containing shared pointers to all of the controller components
		*/
		inline std::vector<std::shared_ptr<ControllerComponent>>& getControllers() { return m_controllers; }

		//! Function to get the predraw render commands in the layer
		/*!
		\return A vector containing shared pointers to all of the predraw render commands
		*/
		inline std::vector<std::shared_ptr<RenderCommand>>& getPredrawCommands() { return m_predrawCommands; }
		//! Function to get the postdraw render commands in the layer
		/*!
		\return A vector containing shared pointers to all of the postdraw render commands
		*/
		inline std::vector<std::shared_ptr<RenderCommand>>& getPostdrawCommands() { return m_postdrawCommands; }
		//! Function to get the initialization render commands in the layer
		/*!
		\return A vector containing shared pointers to all of the initialization render commands
		*/
		inline std::vector<std::shared_ptr<RenderCommand>>& getInitCommands() { return m_initCommands; }
		//! Function to get the exit render commands in the layer
		/*!
		\return A vector containing shared pointers to all of the exit render commands
		*/
		inline std::vector<std::shared_ptr<RenderCommand>>& getExitCommands() { return m_exitCommands; }

		//! Function to get the misc data of the layer
		/*!
		\return A list of void pointers to data
		*/
		inline std::list<void*>& getData() { return m_data; }

		//! Function to get the cameras view
		/*!
		\return The current cameras view
		*/
		inline glm::mat4& getCamView() { return m_camView; }
		//! Function to get the cameras projection
		/*!
		\return The current cameras projection
		*/
		inline glm::mat4& getCamProj() { return m_camProj; }
		//! Function to get the cameras view projection
		/*!
		\return The current cameras view projection
		*/
		inline glm::mat4& getCamViewProj() { return m_camViewProj; }
		//! Function to get the cameras position
		/*!
		\return The current cameras position
		*/
		inline glm::vec3 getCamPos() { return m_camPos; }
	};
}