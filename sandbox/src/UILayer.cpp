/** \file UILayer.cpp
*/
#include <Engine.h>
#include "UILayer.h"

void UILayer::onAttach()
{
	m_pRenderer.reset(Engine::Renderer::createText());

	std::unordered_multimap<std::string, unsigned int> map;
	std::string fontFilepath = "assets/fonts/arial_narrow_7.ttf";
	map.insert(std::make_pair(fontFilepath, 64));
	m_pResources->populateCharacters(map);

	m_pLabel.reset(Engine::TextLabel::create(fontFilepath, 64, "Footer", glm::vec2(50, 500), 0, 1, glm::vec3(0.2f, 0.8f, 0.2f)));
	m_pLabel2.reset(Engine::TextLabel::create(fontFilepath, 64, "Level 1", glm::vec2(250, 50), 0, 1.2, glm::vec3(0.8f, 0.f, 0.2f)));

	m_materials.push_back(std::make_shared<Engine::MaterialComponent>(Engine::MaterialComponent(m_pLabel->getMaterial())));
	m_positions.push_back(std::make_shared<Engine::PositionComponent>(Engine::PositionComponent(
		glm::vec3(m_pLabel->getPosition(), 0), glm::vec3(0, m_pLabel->getOrientation(), 0), glm::vec3(m_pLabel->getScale()))));
	m_textures.push_back(std::make_shared<Engine::TextureComponent>(Engine::TextureComponent(m_pResources->getFontTexture())));

	m_gameObjects.push_back(std::make_shared<Engine::GameObject>());
	m_gameObjects.back()->addComponent(m_materials.back());
	m_gameObjects.back()->addComponent(m_positions.back());
	m_gameObjects.back()->addComponent(m_textures.back());


	m_materials.push_back(std::make_shared<Engine::MaterialComponent>(Engine::MaterialComponent(m_pLabel2->getMaterial())));
	m_positions.push_back(std::make_shared<Engine::PositionComponent>(Engine::PositionComponent(
		glm::vec3(m_pLabel2->getPosition(), 0), glm::vec3(0, m_pLabel2->getOrientation(), 0), glm::vec3(m_pLabel2->getScale()))));
	m_textures.push_back(std::make_shared<Engine::TextureComponent>(Engine::TextureComponent(m_pResources->getFontTexture())));

	m_gameObjects.push_back(std::make_shared<Engine::GameObject>());
	m_gameObjects.back()->addComponent(m_materials.back());
	m_gameObjects.back()->addComponent(m_positions.back());
	m_gameObjects.back()->addComponent(m_textures.back());


	m_pUBO = m_pResources->getUBO("TextMatrices");

	m_pCamera.reset(new Engine::FreeOrthoCameraController2D);
	m_pCamera->init(0.f, 0.f, 800.f, 600.f);
}

void UILayer::onDetach()
{

}

void UILayer::onUpdate(float timestep)
{
	m_pCamera->onUpdate(timestep);

	for (auto& CGO : m_gameObjects)
		CGO->onUpdate(timestep);

	std::vector<void*> tempData;
	// Add Matrices data to vector
	tempData.push_back((void*)&m_pCamera->getCamera()->getViewProjection()[0][0]);
	m_sceneData.insert(std::make_pair(m_pUBO, tempData));

	m_pRenderer->actionCommand(Engine::RenderCommand::setOneMinusAlphaBlendingCommand(true));

	m_pRenderer->beginScene(m_sceneData);

	m_pLabel->getMaterial()->setDataElement("u_fontColour", (void*)&m_pLabel->getColour()[0]);
	m_pLabel2->getMaterial()->setDataElement("u_fontColour", (void*)&m_pLabel2->getColour()[0]);

	for (auto& mat : m_materials)
		m_pRenderer->submit(mat->getMaterial());

	m_pRenderer->actionCommand(Engine::RenderCommand::setOneMinusAlphaBlendingCommand(false));
}

void UILayer::onEvent(Engine::Event& event)
{
	m_pCamera->onEvent(event);

	for (auto& CGO : m_gameObjects)
		CGO->onEvent(event);
}