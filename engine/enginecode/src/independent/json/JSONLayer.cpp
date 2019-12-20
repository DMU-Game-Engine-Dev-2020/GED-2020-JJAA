#include "engine_pch.h"

#include "json/JSONLayer.h"
#include "json/JSONLoader.h"

namespace Engine
{
	void JSONLayer::onAttach()
	{
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
		// update camera
		m_pCamera->onUpdate(timestep);

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
		m_pCamera->onEvent(event);
		for (auto& CGO : m_gameObjects) CGO->onEvent(event);
	}
}