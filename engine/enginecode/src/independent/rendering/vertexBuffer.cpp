#include "engine_pch.h"

#include "rendering/renderAPI.h"
#include "rendering/vertexBuffer.h"
#include "platform/openGL/OpenGLVertexBuffer.h"
#include "systems/log.h"

namespace Engine
{
	VertexBuffer* VertexBuffer::create(float* vertices, unsigned int size, BufferLayout& layout)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			LOG_CRITICAL("Lack of graphics API not supported");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLVertexBuffer(vertices, size, layout);
			break;
		case RenderAPI::API::Direct3D:
			LOG_CRITICAL("Direct 3D not supported");
			break;
		default:
			LOG_CRITICAL("Unknown graphics API");
		}
	}
}