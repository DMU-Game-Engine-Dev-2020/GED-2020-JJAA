#include "engine_pch.h"

#include "rendering/renderAPI.h"
#include "rendering/texture.h"
#include "platform/openGL/OpenGLTexture.h"
#include "systems/log.h"

namespace Engine
{
	Texture* Texture::createFromFile(const std::string& filepath)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None: // If no render API
			LOG_CRITICAL("Lack of graphics API not supported"); // Log what's happening
			break;
		case RenderAPI::API::OpenGL: // If using openGL
			return new OpenGLTexture(filepath); // Create and return a pointer to an openGL texture class
			break;
		case RenderAPI::API::Direct3D: // If using direct3D
			LOG_CRITICAL("Direct 3D not supported"); // Log what's happening
			break;
		default: // If not recognised
			LOG_CRITICAL("Unknown graphics API"); // Log what's happening
		}
	}

	Texture* Texture::createFromRawData(unsigned int width, unsigned int height, unsigned int channels, unsigned char* texData)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None: // If no render API
			LOG_CRITICAL("Lack of graphics API not supported"); // Log what's happening
			break;
		case RenderAPI::API::OpenGL: // If using openGL
			return new OpenGLTexture(width, height, channels, texData); // Create and return a pointer to an openGL texture class
			break;
		case RenderAPI::API::Direct3D: // If using direct3D
			LOG_CRITICAL("Direct 3D not supported"); // Log what's happening
			break;
		default: // If not recognised
			LOG_CRITICAL("Unknown graphics API"); // Log what's happening
		}
	}
}