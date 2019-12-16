/** \file UILayer.h
*/
#pragma once

/**
\class UILayer
\brief Layer for 2D things
*/
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
	//! Constructor
	/*!
	\param name The name of the layer
	*/
	UILayer(const std::string& name = "Layer");

	//! Function called when the layer is added
	void onAttach() override;
	//! Function called when the layer is removed
	void onDetach() override;
	//! Function to update the layer every frame
	/*!
	\param timestep The time since the previous frame
	*/
	void onUpdate(float timestep) override;
	//! Function to handle events
	/*!
	\param event The event
	*/
	void onEvent(Engine::Event& event) override;
};