#include "engine_pch.h"

#include "platform/openGL/OpenGLTexture.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace Engine
{
	unsigned int OpenGLTexture::ms_iTextureSlot = 0;
	std::list<unsigned int> OpenGLTexture::ms_deletedSlots;

	OpenGLTexture::OpenGLTexture(const std::string& filepath)
	{
		genTex();
		wrapTex();

		int width, height, channels;

		unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
		m_iWidth = width;
		m_iHeight = height;
		m_iChannels = channels;

		setChannels(data);

		stbi_image_free(data);
	}

	OpenGLTexture::OpenGLTexture(unsigned int width, unsigned int height, unsigned int channels, unsigned char* texData)
		: m_iWidth(width), m_iHeight(height), m_iChannels(channels)
	{
		genTex();
		wrapTex();

		setChannels(texData);
	}

	OpenGLTexture::~OpenGLTexture()
	{
		glDeleteTextures(1, &m_iTextureID);

		ms_deletedSlots.push_front(m_iTextureSlot);
	}

	void OpenGLTexture::genTex()
	{
		glGenTextures(1, &m_iTextureID);

		if (ms_deletedSlots.empty())
		{
			m_iTextureSlot = ms_iTextureSlot;
			ms_iTextureSlot++;
		}
		else
		{
			m_iTextureSlot = ms_deletedSlots.front();
			ms_deletedSlots.pop_front();
		}

		glActiveTexture(GL_TEXTURE0 + m_iTextureSlot);
		glBindTexture(GL_TEXTURE_2D, m_iTextureID);
	}

	void OpenGLTexture::wrapTex()
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	void OpenGLTexture::setChannels(unsigned char* data)
	{
		if (data)
		{
			if (m_iChannels == 3)
			{
				glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_iWidth, m_iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
			}
			else if (m_iChannels == 4)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			}
			else if (m_iChannels == 1)
			{
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_iWidth, m_iHeight, 0, GL_RED, GL_UNSIGNED_BYTE, data);
				glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
			}
			else return;

			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else return;
	}
}