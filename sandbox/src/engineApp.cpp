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

