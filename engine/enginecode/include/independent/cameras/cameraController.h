#pragma once

#include "camera.h"
#include "events/event.h"
#include "events/windowEvents.h"
#include "events/mouseEvents.h"

namespace Engine
{
	class CameraController
	{
	public:
		virtual void init(float, float, float, float) = 0;
		virtual std::shared_ptr<Camera> getCamera() = 0;
		virtual void onUpdate(float timestep) = 0;
		virtual void onEvent(Event& e) = 0;
		virtual bool onResize(WindowResizeEvent& e) = 0;
	};
}