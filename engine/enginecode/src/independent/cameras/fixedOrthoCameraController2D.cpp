/** \file fixedOrthoCameraController2D.cpp
*/
#include "engine_pch.h"

#include "cameras/fixedOrthoCameraController2D.h"

namespace Engine
{
	void FixedOrthoCameraController2d::init(float left, float top, float width, float height)
	{
		// Create a new camera
		m_camera.reset(new OrthographicCamera2D(left, left + width, top + height, top));
		m_position = glm::vec2(left, top); // Set the position using values passed in
		m_camera->setPosition(m_position); // Give the position to the camera
	}
}