#include "engine_pch.h"

#include "rendering/renderAPI.h"
#include "rendering/indexBuffer.h"
#include "platform/openGL/OpenGLIndexBuffer.h"
#include "systems/log.h"

namespace Engine
{
	IndexBuffer* IndexBuffer::create(unsigned int* indices, unsigned int size)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			LOG_CRITICAL("Lack of graphics API not supported");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLIndexBuffer(indices, size);
			break;
		case RenderAPI::API::Direct3D:
			LOG_CRITICAL("Direct 3D not supported");
			break;
		default:
			LOG_CRITICAL("Unknown graphics API");
		}
	}
}