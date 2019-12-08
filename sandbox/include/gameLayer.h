//#pragma once
//
//#include <Engine.h>
//
//class GameLayer : public Engine::Layer
//{
//private:
//	std::shared_ptr<Engine::Material> m_pFCcube;
//	std::shared_ptr<Engine::Material> m_pTPcube;
//	std::shared_ptr<Engine::UniformBuffer> m_pUBOMatrices;
//	std::shared_ptr<Engine::UniformBuffer> m_pUBOLights;
//	std::shared_ptr<Engine::Texture> m_pNumberTex;
//	std::shared_ptr<Engine::Texture> m_pLetterTex;
//	glm::mat4 m_FCmodel;
//	glm::mat4 m_TPmodel;
//	unsigned int m_iTexSlot;
//	glm::vec3 m_lightColour;
//	glm::vec3 m_lightPosition;
//	glm::vec3 m_viewPosition;
//	Engine::SceneData m_sceneData;
//	bool m_bGoingUp = false;
//	float m_fTimeSummed = 10.f;
//public:
//	GameLayer(const std::string& name = "Layer");
//
//	void onAttach() override;
//	void onDetach() override;
//	void onUpdate(float timestep) override;
//	void onEvent(Engine::Event& event) override;
//};