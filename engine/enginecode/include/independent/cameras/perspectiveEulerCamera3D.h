/** \file perspectiveEulerCamera3D.h
*/
#pragma once

#include "camera.h"

namespace Engine
{
	/**
	\class PerspectiveEulerCamera3D
	\brief 3 dimensional camera to move around in world space 
	*/
	class PerspectiveEulerCamera3D : public Camera
	{
	private:
		glm::vec3 m_position = glm::vec3(0.f); //!< The position in the world space

		//! Function to update the cameras view, overridden from abstract one in Camera, not used
		void updateView() override {}
	public:
		//! Constructor
		/*!
		\param fov The field of view
		\param aspectRatio The cameras aspect ratio
		\param nearClip Closest something can be drawn
		\param farClip Furthest something can be drawn
		*/
		PerspectiveEulerCamera3D(float fov, float aspectRatio, float nearClip, float farClip);

		//! Function to reset the camera
		/*!
		\param fov The field of view
		\param aspectRatio The cameras aspect ratio
		\param nearClip Closest something can be drawn
		\param farClip Furthest something can be drawn
		*/
		void reset(float fov, float aspectRatio, float nearClip, float farClip);

		//! Function to get the position of the camera
		/*!
		\return The cameras position
		*/
		inline glm::vec3 getPosition() { return m_position; }
		//! Function to set the cameras view
		/*!
		\param view The new view
		*/
		void setView(const glm::mat4& view) { m_view = view; m_viewProjection = m_projection * m_view; }
	};
}