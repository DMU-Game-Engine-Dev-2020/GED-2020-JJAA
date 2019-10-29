#include "engine_pch.h"

#include "rendering/renderAPI.h"
#include "rendering/vertexArray.h"
#include "platform/openGL/OpenGLVertexArray.h"
#include "systems/log.h"

namespace Engine
{
	VertexArray* VertexArray::create()
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			LOG_CRITICAL("Lack of graphics API not supported");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLVertexArray;
			break;
		case RenderAPI::API::Direct3D:
			LOG_CRITICAL("Direct 3D not supported");
			break;
		default:
			LOG_CRITICAL("Unknown graphics API");
		}
	}
}