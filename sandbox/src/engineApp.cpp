/** \file engineApp.cpp
*/

#include "engineApp.h"

//#include "gameLayer.h"
//#include "UILayer.h"


engineApp::engineApp()
{
	// Create setup object
	m_setup = Engine::JSONSetup(&m_levels);
	m_setup.load("assets/levelControl/levelControl.json"); // Load the level information
	// Load the first level
	loadLevel(m_levels.begin());

	//m_pLayerStack->push(std::make_shared<GameLayer>(GameLayer("Game Layer")));
	//m_pLayerStack->push(std::make_shared<UILayer>(UILayer("UI Layer")));

	//m_pLayerStack->push(std::make_shared<Engine::JSONLayer>(Engine::JSONLayer("assets/layers/lvl2GameLayer.json", "Game Layer")));
	//m_pLayerStack->push(std::make_shared<Engine::JSONLayer>(Engine::JSONLayer("assets/layers/lvl2UILayer.json", "UI Layer")));
	//m_pLayerStack->push(std::make_shared<Engine::JSONLayer>(Engine::JSONLayer("assets/layers/lvl2UILayer2.json", "UI Layer 2")));
}

engineApp::~engineApp()
{

}

Engine::Application* Engine::startApplication()
{
	return new engineApp();
}

void Engine::Application::loadLevel(Levels::iterator& layers)
{
	// While the layer stack has layers in it
	while (m_pLayerStack->begin() != m_pLayerStack->end())
	{
		// Remove a layer
		m_pLayerStack->pop();
	}
	// For each layer in the new level
	for (auto& layer : layers->first)
	{
		// Push the layer onto the stack
		m_pLayerStack->push(std::make_shared<Engine::JSONLayer>(Engine::JSONLayer(layer.first, layer.second)));
	}
	// Set which level key code is active
	m_iCurrentLevel = layers->second;
}