/** \file renderAPI.cpp
*/
#include "engine_pch.h"
#include "rendering/renderAPI.h"

namespace Engine
{
	// Set the render API to openGL
	RenderAPI::API RenderAPI::ms_API = RenderAPI::API::OpenGL;
}