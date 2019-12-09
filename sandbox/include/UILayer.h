//#pragma once
//
//#include <Engine.h>
//
//class UILayer : public Engine::Layer
//{
//private:
//	std::shared_ptr<Engine::Material> m_pMat;
//	std::shared_ptr<Engine::Texture> m_pTexture;
//
//	unsigned int m_iTexSlot;
//public:
//	UILayer(const std::string& name = "Layer");
//
//	void onAttach() override;
//	void onDetach() override;
//	void onUpdate(float timestep) override;
//	void onEvent(Engine::Event& event) override;
//};