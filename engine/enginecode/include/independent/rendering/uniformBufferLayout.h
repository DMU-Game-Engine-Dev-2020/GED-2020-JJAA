#pragma once

#include "bufferElement.h"

#include <vector>

namespace Engine
{
	class UniformBufferLayout
	{
	private:
		std::vector<BufferElement> m_elements; //!< The buffer elements
		unsigned int m_iStride; //!< The stride
		unsigned int m_iSize;

		void calcStrideAndOffsets(); //!< Used to calculate the stride and the offsets
	public:
		UniformBufferLayout() {}
		UniformBufferLayout(const std::initializer_list<BufferElement>& elements);

		inline unsigned int getStride() const { return m_iStride; }
		inline unsigned int getSize() const { return m_iSize; }

		std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_elements.end(); }

		void addElement(ShaderDataType dataType);
	};
}