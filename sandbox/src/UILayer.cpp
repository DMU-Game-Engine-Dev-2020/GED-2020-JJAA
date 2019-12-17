/** \file UILayer.cpp
*/
#include <Engine.h>
#include "UILayer.h"

UILayer::UILayer(const std::string& name) : Layer(name)
{
	m_pRenderer.reset(Engine::Renderer::createText());

	std::unordered_map<std::string, unsigned int> map;
	map.insert(std::make_pair("assets/fonts/arial_narrow_7.ttf", 126));
	m_pResources->populateCharacters(map);

	m_pLabel.reset(Engine::TextLabel::create("arial_narrow_7", 126, "M", glm::vec2(100, 200), 0, 1, glm::vec3(1.0f, 0.3f, 0.6f)));

	m_pMat = m_pLabel->getMaterial();

	m_UBOs.push_back(m_pResources->getUBO("TextMatrices"));

	m_pCamera.reset(new Engine::FreeOrthoCameraController2D);
	m_pCamera->init(0.f, 0.f, 800.f, 600.f);
}

void UILayer::onAttach()
{

}

void UILayer::onDetach()
{

}

void UILayer::onUpdate(float timestep)
{
	m_pCamera->onUpdate(timestep);


	std::vector<void*> tempData;
	// Add Matrices data to vector
	tempData.push_back((void*)&m_pCamera->getCamera()->getViewProjection()[0][0]);

	m_sceneData.insert(std::make_pair(m_UBOs.back(), tempData));

	m_pRenderer->beginScene(m_sceneData);

	m_model = m_pLabel->getModel();
	m_iTexSlot = m_pResources->getFontTexture()->getSlot();
	m_colour = m_pLabel->getColour();

	m_pMat->setDataElement("u_model", (void*)&m_model[0][0]);
	m_pMat->setDataElement("u_texData", (void*)&m_iTexSlot);
	m_pMat->setDataElement("u_fontColour", (void*)&m_colour[0]);

	//m_pRenderer->actionCommand(Engine::RenderCommand::clearColourBufferCommand());
	m_pRenderer->actionCommand(Engine::RenderCommand::setOneMinusAlphaBlendingCommand(true));

	m_pRenderer->submit(m_pMat);

	m_pRenderer->actionCommand(Engine::RenderCommand::setOneMinusAlphaBlendingCommand(false));
}

void UILayer::onEvent(Engine::Event& event)
{

}