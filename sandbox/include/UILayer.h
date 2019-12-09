#pragma once

class UILayer : public Engine::Layer
{
private:
	std::shared_ptr<Engine::Material> m_pMat;
	std::shared_ptr<Engine::Texture> m_pTexture;
	std::shared_ptr<Engine::TextLabel> m_pLabel;

	glm::mat4 m_model;
	unsigned int m_iTexSlot;
	glm::vec3 m_colour;
public:
	UILayer(const std::string& name = "Layer");

	void onAttach() override;
	void onDetach() override;
	void onUpdate(float timestep) override;
	void onEvent(Engine::Event& event) override;
};