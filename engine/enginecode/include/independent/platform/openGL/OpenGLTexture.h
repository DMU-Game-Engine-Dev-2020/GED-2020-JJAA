#pragma once

#include "rendering/texture.h"

#include <list>

namespace Engine
{
	class OpenGLTexture : public Texture
	{
	private:
		unsigned int m_iTextureID;

		unsigned int m_iWidth;
		unsigned int m_iHeight;
		unsigned int m_iChannels;
		unsigned int m_iTextureSlot;

		static unsigned int s_iTextureSlot;
		static std::list<unsigned int> s_deletedSlots;

		void genTex();
		void wrapTex();
		void setChannels(unsigned char* data);
	public:
		OpenGLTexture(const std::string& filepath);
		OpenGLTexture(unsigned int width, unsigned int height, unsigned int channels, unsigned char* texData);
		~OpenGLTexture();

		inline unsigned int getWidth() const override { return m_iWidth; }
		inline unsigned int getHeight() const override { return m_iHeight; }
		inline unsigned int getChannels() const override { return m_iChannels; }
		inline unsigned int getSlot() const override { return m_iTextureSlot; }
	};
}