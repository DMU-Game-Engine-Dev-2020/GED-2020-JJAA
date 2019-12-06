#pragma once

#include "cameraController.h"
#include "perspectiveEulerCamera3D.h"

namespace Engine
{
	class FPSCameraControllerEuler : public CameraController
	{
	private:
		std::shared_ptr<PerspectiveEulerCamera3D> m_camera;
		glm::mat4 m_view;
		glm::vec3 m_position;
		glm::vec3 m_forward;
		glm::vec3 m_up;
		glm::vec3 m_right;
		glm::vec3 m_worldUp;
		float m_fYaw;
		float m_fPitch;
		float m_fTranslationSpeed = 2.f;
		float m_fRotationSpeed = 2.f;
		glm::vec2 m_lastMousePosition;
		
		void updateView();
	public:
		FPSCameraControllerEuler();

		void init(float fov = 45.f, float aspectRatio = 4.f / 3.f, float nearClip = 0.1f, float farClip = 100.f) override;
		std::shared_ptr<Camera> getCamera() override { return m_camera; }
		void onUpdate(float timestep) override;
		void onEvent(Event& e) override {}
		bool onResize(WindowResizeEvent& e) override { return true; }
	};
}