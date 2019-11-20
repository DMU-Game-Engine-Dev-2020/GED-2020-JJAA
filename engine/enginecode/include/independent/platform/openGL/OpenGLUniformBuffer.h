#pragma once

#include "rendering/uniformBuffer.h"

namespace Engine
{
	class OpenGLUniformBuffer : public UniformBuffer
	{
	private:
		unsigned int m_id;
		unsigned int m_iBindingPoint;
		UniformBufferLayout m_layout;

		static unsigned int s_bindingPoint;
	public:
		OpenGLUniformBuffer(unsigned int size, UniformBufferLayout& layout);
		OpenGLUniformBuffer(unsigned int size, unsigned int rangeStart, unsigned int rangeEnd, UniformBufferLayout& layout);
		~OpenGLUniformBuffer();

		void bind() override;
		void unbind() override;
		void attachShaderBlock(const std::shared_ptr<Shader>& shader, const std::string& blockName) override;
		void setData(unsigned int offset, unsigned int size, void* data) override;

		inline const UniformBufferLayout& getLayout() const override { return m_layout; }
	};
}