#include "engine_pch.h"

#include "cameras/perspectiveEulerCamera3D.h"

namespace Engine
{
	PerspectiveEulerCamera3D::PerspectiveEulerCamera3D(float fov, float aspectRatio, float nearClip, float farClip)
	{
		reset(fov, aspectRatio, nearClip, farClip);
	}

	void PerspectiveEulerCamera3D::reset(float fov, float aspectRatio, float nearClip, float farClip)
	{
		m_projection = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
		m_view = glm::mat4(1.f);
		m_viewProjection = m_projection * m_view;
	}
}