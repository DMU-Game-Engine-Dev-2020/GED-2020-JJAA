#pragma once

#include "rendering/shaderData.h"

#include <string>
#include <functional>

namespace Engine
{
	class UniformObject
	{
	protected:
		using UploadFunc = std::function<bool(int, void*)>;
	public:
		virtual void setLocationAndFunction(unsigned int shaderID, UploadFunc& func) = 0;

		virtual ShaderDataType getType() = 0;
		virtual void uniformUpload(void* data) = 0;
	};
}