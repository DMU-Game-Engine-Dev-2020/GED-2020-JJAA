#include "engine_pch.h"

#include "rendering/bufferLayout.h"

namespace Engine
{
	BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& elements) : m_elements(elements)
	{
		calcStrideAndOffsets();
	}

	void BufferLayout::addElement(ShaderDataType dataType)
	{
		m_elements.push_back(BufferElement(dataType));
		calcStrideAndOffsets();
	}

	void BufferLayout::calcStrideAndOffsets()
	{
		unsigned int offset = 0;
		m_iStride = 0;
		for (auto& element : m_elements)
		{
			element.m_iOffset = offset;
			offset += element.m_iSize;
			m_iStride += element.m_iSize;
		}
	}
}