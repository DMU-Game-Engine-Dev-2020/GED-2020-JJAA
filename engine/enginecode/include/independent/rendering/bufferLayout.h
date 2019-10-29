#pragma once

#include "bufferElement.h"

#include <vector>

namespace Engine
{
	class BufferLayout
	{
	private:
		std::vector<BufferElement> m_elements;
		unsigned int m_iStride;
		void calcStrideAndOffsets();
	public:
		BufferLayout() {}
		BufferLayout(const std::initializer_list<BufferElement>& elements);

		inline unsigned int getStride() const { return m_iStride; }

		std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_elements.end(); }

		void addElement(ShaderDataType dataType);
	};
}