#pragma once

#include "bufferLayout.h"

namespace Engine
{
	class VertexBuffer
	{
	public:
		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual void edit(float* vertices, unsigned int size, unsigned int offset) = 0;
		virtual const BufferLayout& getLayout() const = 0;

		static VertexBuffer* create(float* vertices, unsigned int size, BufferLayout& layout);
	};
}