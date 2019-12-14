#include "engine_pch.h"

#include "cameras/freeOrthoCameraController2D.h"
#include "windows/inputPoller.h"
#include "core/codes.h"

namespace Engine
{
	void FreeOrthoCameraController2D::init(float left, float top, float width, float height)
	{
		m_camera.reset(new OrthographicCamera2D(left, left + width, top + height, top));
		m_position = glm::vec2(left, top);
		m_camera->setPosition(m_position);
	}

	void FreeOrthoCameraController2D::onUpdate(float timestep)
	{
		if (InputPoller::isKeyPressed(ENGINE_KEY_W))
		{
			m_position.x += -sin(glm::radians(m_fRotation) * m_fTranslationSpeed * timestep);
			m_position.y += cos(glm::radians(m_fRotation) * m_fTranslationSpeed * timestep);
		}
		if (InputPoller::isKeyPressed(ENGINE_KEY_S))
		{
			m_position.x -= -sin(glm::radians(m_fRotation) * m_fTranslationSpeed * timestep);
			m_position.y -= cos(glm::radians(m_fRotation) * m_fTranslationSpeed * timestep);
		}
		if (InputPoller::isKeyPressed(ENGINE_KEY_A))
		{
			m_position.x += cos(glm::radians(m_fRotation) * m_fTranslationSpeed * timestep);
			m_position.y += sin(glm::radians(m_fRotation) * m_fTranslationSpeed * timestep);
		}
		if (InputPoller::isKeyPressed(ENGINE_KEY_D))
		{
			m_position.x -= cos(glm::radians(m_fRotation) * m_fTranslationSpeed * timestep);
			m_position.y -= sin(glm::radians(m_fRotation) * m_fTranslationSpeed * timestep);
		}

		if (InputPoller::isKeyPressed(ENGINE_KEY_Q))
		{
			m_fRotation -= m_fRotationSpeed * timestep;
			if (m_fRotation > 180.f)
				m_fRotation -= 360.f;
			else if (m_fRotation <= -180.f)
				m_fRotation += 360.f;
		}

		if (InputPoller::isKeyPressed(ENGINE_KEY_E))
		{
			m_fRotation += m_fRotationSpeed * timestep;
			if (m_fRotation > 180.f)
				m_fRotation -= 360.f;
			else if (m_fRotation <= -180.f)
				m_fRotation += 360.f;
		}

		m_camera->setPositionAndRotation(m_position, m_fRotation);
	}
}