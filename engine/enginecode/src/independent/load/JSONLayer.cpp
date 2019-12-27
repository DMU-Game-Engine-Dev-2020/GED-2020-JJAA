/** \file JSONLayer.cpp
*/
#include "engine_pch.h"

#include "load/JSONLayer.h"
#include "load/JSONLoader.h"

#include "core/codes.h"

namespace Engine
{
	void JSONLayer::onAttach()
	{
		// Load the layer
		JSONLoader::load(m_filepath, *this);

		// Call all init render commands
		for (auto& renderCommand : m_initCommands) 
			m_pRenderer->actionCommand(renderCommand.get());
	}

	void JSONLayer::onDetach()
	{
		// Call exit render commands
		for (auto& renderCommand : m_exitCommands) m_pRenderer->actionCommand(renderCommand.get());

		for (auto& ptr : m_data)
		{
			delete ptr;
		}
	}

	void JSONLayer::onUpdate(float timestep)
	{
		// If there is a player component
		if (m_player != nullptr)
		{
			// Get the cameras
			auto cameras = m_player->getCameras();
			cameras.first->onUpdate(timestep);
			// If there is a second camera
			if (cameras.second != nullptr)
				cameras.second->onUpdate(timestep);
		}
		else
			m_pCamera->onUpdate(timestep);

		// Set camera variables used for uniform buffers
		m_camView = m_pCamera->getCamera()->getView();
		m_camProj = m_pCamera->getCamera()->getProjection();
		m_camViewProj = m_pCamera->getCamera()->getViewProjection();
		m_camPos = m_pCamera->getCamera()->getPosition();
		
		// Update Game Objects
		for (auto& CGO : m_gameObjects) CGO->onUpdate(timestep);

		// Call predraw render commands
		for (auto& renderCommand : m_predrawCommands) 
			m_pRenderer->actionCommand(renderCommand.get());

		m_pRenderer->beginScene(m_sceneData);
		for (auto& mat : m_materials) m_pRenderer->submit(mat->getMaterial());

		// Call postdraw render commands
		for (auto& renderCommand : m_postdrawCommands) 
			m_pRenderer->actionCommand(renderCommand.get());
	}

	void JSONLayer::onEvent(Event & event)
	{
		// If there is a player component
		if (m_player != nullptr)
		{
			// Get the cameras
			auto cameras = m_player->getCameras();
			// update camera(s)
			cameras.first->onEvent(event);
			// If there is a second camera
			if (cameras.second != nullptr)
				cameras.second->onEvent(event);
		}
		else
			m_pCamera->onEvent(event);

		for (auto& CGO : m_gameObjects) CGO->onEvent(event);

		// Create event dispatcher
		EventDispatcher dispatcher(event);
		// If the Event type matches, call the corresponding function
		dispatcher.dispatch<KeyPressedEvent>(std::bind(&JSONLayer::onKeyPressed, this, std::placeholders::_1));
	}

	bool JSONLayer::onKeyPressed(KeyPressedEvent& e)
	{
		// If there is a player object in the layer
		if (m_player != nullptr)
		{
			if (e.getKeyCode() == ENGINE_KEY_RIGHT_SHIFT) // If the right shift key is pressed
			{
				m_pCamera = m_player->getCurrentCamera(); // Set the  camera to the current player camera
				return true;
			}
		}
		return false;
	}
}