#include "engine_pch.h"

#include "cameras/orthographicCamera2D.h"

namespace Engine
{
	OrthographicCamera2D::OrthographicCamera2D(float left, float right, float bottom, float top)
	{
		reset(left, right, bottom, top);
	}

	void OrthographicCamera2D::reset(float left, float right, float bottom, float top)
	{
		m_projection = glm::ortho(left, right, bottom, top);
		m_view = glm::mat4(1.f);
		m_viewProjection = m_projection * m_view;
	}

	void OrthographicCamera2D::updateView()
	{
		glm::mat4 translate = glm::translate(glm::mat4(1.f), glm::vec3(m_position, 1.f));
		glm::mat4 rotate = glm::rotate(glm::mat4(1.f), glm::radians(m_fRotation), glm::vec3(0.f, 0.f, 1.f));
		m_view = translate * rotate;

		m_viewProjection = m_projection * m_view;
	}
}