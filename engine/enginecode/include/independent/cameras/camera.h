#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
	class Camera
	{
	protected:
		glm::mat4 m_projection;
		glm::mat4 m_view;
		glm::mat4 m_viewProjection;
	public:
		virtual void updateView() = 0;
		const glm::mat4& getProjection() { return m_projection; }
		const glm::mat4& getView() { return m_view; }
		const glm::mat4& getViewProjection() { return m_viewProjection; }
	};
}