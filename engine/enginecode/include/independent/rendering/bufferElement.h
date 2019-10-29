#pragma once

#include "shaderData.h"

namespace Engine
{
	class BufferElement
	{
	public:
		ShaderDataType m_dataType;
		unsigned int m_iSize;
		unsigned int m_iOffset;
		bool m_bNormalized;

		BufferElement() {}
		BufferElement(ShaderDataType dataType, bool normalized = false) : 
			m_dataType(dataType), m_iSize(ShaderDataTypeSize(dataType)), m_iOffset(0), m_bNormalized(normalized) {}
	};
}