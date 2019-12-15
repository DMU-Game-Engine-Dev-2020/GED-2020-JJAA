#pragma once

class GameLayer : public Engine::Layer
{
private:
	std::vector<std::shared_ptr<Engine::GameObject>> m_gameObjects;
	std::vector<std::shared_ptr<Engine::MaterialComponent>> m_materials;
	std::vector<std::shared_ptr<Engine::PositionComponent>> m_positions;
	std::vector<std::shared_ptr<Engine::VelocityComponent>> m_velocities;
	std::vector<std::shared_ptr<Engine::OscilateComponent>> m_oscilate;
	//std::vector<std::shared_ptr<Engine::TextureComponent>> m_textures;
	std::vector<std::shared_ptr<Engine::TextureSwitchComponent>> m_textureSwitch;

	glm::vec3 m_lightColour;
	glm::vec3 m_lightPosition;
	glm::vec3 m_viewPosition;
public:
	GameLayer(const std::string& name = "Layer");

	void onAttach() override;
	void onDetach() override;
	void onUpdate(float timestep) override;
	void onEvent(Engine::Event& event) override;
};