#include "engine_pch.h"

#include "cameras/FPSCameraControllerEuler.h"
#include "windows/inputPoller.h"
#include "core/codes.h"

namespace Engine
{
	FPSCameraControllerEuler::FPSCameraControllerEuler()
	{
		m_lastMousePosition = InputPoller::getMousePosition();
	}

	void FPSCameraControllerEuler::init(float fov, float aspectRatio, float nearClip, float farClip)
	{
		m_camera.reset(new PerspectiveEulerCamera3D(fov, aspectRatio, nearClip, farClip));
		m_position = glm::vec3(0.f, 0.f, -4.5f);
		m_worldUp = glm::vec3(0.f, 1.f, 0.f);
		m_fYaw = 0;
		m_fPitch = 0;
		updateView();
	}

	void FPSCameraControllerEuler::onUpdate(float timestep)
	{
		if (InputPoller::isKeyPressed(ENGINE_KEY_W))
			m_position += m_forward * m_fTranslationSpeed * timestep;
		if (InputPoller::isKeyPressed(ENGINE_KEY_S))
			m_position -= m_forward * m_fTranslationSpeed * timestep;
		if (InputPoller::isKeyPressed(ENGINE_KEY_A))
			m_position -= m_right * m_fTranslationSpeed * timestep;
		if (InputPoller::isKeyPressed(ENGINE_KEY_D))
			m_position += m_right * m_fTranslationSpeed * timestep;

		glm::vec2 currentMousePosition = InputPoller::getMousePosition();
		glm::vec2 mouseDelta = currentMousePosition - m_lastMousePosition;

		m_fYaw -= mouseDelta.x * m_fRotationSpeed * timestep;
		m_fPitch -= mouseDelta.y * m_fRotationSpeed * timestep;

		if (m_fPitch > 89.f)
			m_fPitch = 89.f;
		if (m_fPitch < -89.f)
			m_fPitch = -89.f;

		m_lastMousePosition = currentMousePosition;

		updateView();
	}

	void FPSCameraControllerEuler::updateView()
	{
		glm::vec3 forward;
		forward.x = sin(glm::radians(m_fYaw)) * cos(glm::radians(m_fPitch));
		forward.y = sin(glm::radians(m_fPitch));
		forward.z = cos(glm::radians(m_fYaw)) * cos(glm::radians(m_fPitch));
		
		m_forward = glm::normalize(forward);

		m_right = glm::normalize(glm::cross(m_forward, m_worldUp));
		m_up = glm::normalize(glm::cross(m_right, m_forward));

		m_view = glm::lookAt(m_position, m_position + m_forward, m_up);

		m_camera->setView(m_view);
	}
}
