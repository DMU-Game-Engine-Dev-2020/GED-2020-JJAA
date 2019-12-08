#pragma once

#include "cameraController.h"
#include "orthographicCamera2D.h"

namespace Engine
{
	class FreeOrthoCameraController2D : public CameraController
	{
	private:
		std::shared_ptr<OrthographicCamera2D> m_camera;
		glm::vec2 m_position = glm::vec2(0.f);
		float m_fRotation = 0.f;
		float m_fTranslationSpeed = 100.f;
		float m_fRotationSpeed = 10.f;
	public:
		FreeOrthoCameraController2D();

		void init(float left, float top, float width, float height) override;
		inline std::shared_ptr<Camera> getCamera() override { return m_camera; }
		void onUpdate(float timestep) override;
		void onEvent(Event& e) override {}
		bool onResize(WindowResizeEvent& e) override { return true; }
	};
}