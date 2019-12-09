/** \file engineApp.cpp
*/

#include "engineApp.h"

#include "gameLayer.h"
#include "UILayer.h"


engineApp::engineApp()
{
	m_pLayerStack->push(std::make_shared<GameLayer>(GameLayer("Game Layer")));
	m_pLayerStack->push(std::make_shared<UILayer>(UILayer("UI Layer")));
}

engineApp::~engineApp()
{

}

Engine::Application* Engine::startApplication()
{
	return new engineApp();
}