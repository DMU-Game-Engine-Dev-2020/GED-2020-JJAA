#pragma once

#include "cameraController.h"
#include "perspectiveEulerCamera3D.h"

#include "events/mouseEvents.h"

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
		float m_fTranslationSpeed = 5.f;
		float m_fRotationSpeed = 4.f;
		glm::vec2 m_lastMousePosition;
		
		void updateView();
	public:
		FPSCameraControllerEuler();

		void init(float fov, float aspectRatio, float nearClip, float farClip) override;
		inline std::shared_ptr<Camera> getCamera() override { return m_camera; }
		void onUpdate(float timestep) override;
		void onEvent(Event& e) override;
		bool onResize(WindowResizeEvent& e) override { return true; }
		bool onMouseButtonPressed(MouseButtonPressedEvent& e);
	};
}