#pragma once

#include "camera.h"

#include <glm/gtc/quaternion.hpp>

namespace Engine
{
	class PerspectiveEulerCamera3D : public Camera
	{
	private:
		glm::vec3 m_position = glm::vec3(0.f);

		void updateView() override {}
	public:
		PerspectiveEulerCamera3D(float fov, float aspectRatio, float nearClip, float farClip);

		void reset(float fov, float aspectRatio, float nearClip, float farClip);

		inline glm::vec3 getPosition() { return m_position; }
		void setView(const glm::mat4& view) { m_view = view; m_viewProjection = m_projection * m_view; }
	};
}