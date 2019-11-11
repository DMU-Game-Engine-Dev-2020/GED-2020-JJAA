/** \file OpenGLTexture.cpp
*/
#include "engine_pch.h"

#include "platform/openGL/OpenGLTexture.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace Engine
{
	// Initialize static variables
	unsigned int OpenGLTexture::s_iTextureSlot = 0;
	std::list<unsigned int> OpenGLTexture::s_deletedSlots;

	OpenGLTexture::OpenGLTexture(const std::string& filepath)
	{
		// Generate and wrap the texture
		genTex(); 
		wrapTex();

		int width, height, channels; // Ints to be used to get the width, height and the channels
		// Load the texture data
		unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
		// Set the class variables
		m_iWidth = width;
		m_iHeight = height;
		m_iChannels = channels;

		setChannels(data); // Set the channels

		stbi_image_free(data); // Free the loaded image
	}

	OpenGLTexture::OpenGLTexture(unsigned int width, unsigned int height, unsigned int channels, unsigned char* texData)
		: m_iWidth(width), m_iHeight(height), m_iChannels(channels)
	{
		// Generate and wrap the texture
		genTex();
		wrapTex();

		setChannels(texData); // Set the channels
	}

	OpenGLTexture::~OpenGLTexture()
	{
		glDeleteTextures(1, &m_iTextureID); // Delete the texture

		s_deletedSlots.push_front(m_iTextureSlot); // Add the value of the texture slot to the list so it can be used again
	}

	void OpenGLTexture::genTex()
	{
		glGenTextures(1, &m_iTextureID); // Generate the texture

		// If the list of available slots is empty
		if (s_deletedSlots.empty())
		{
			// Set the texture slot to the static one
			m_iTextureSlot = s_iTextureSlot;
			s_iTextureSlot++; // Increase the static one
		}
		else // If there is an available slot in the list
		{
			// Set the texture slot to the front one in the list
			m_iTextureSlot = s_deletedSlots.front();
			s_deletedSlots.pop_front(); // Remove the front one from the list
		}

		glActiveTexture(GL_TEXTURE0 + m_iTextureSlot); // Set the current slot to the active one
		glBindTexture(GL_TEXTURE_2D, m_iTextureID); // Bind the texture
	}

	void OpenGLTexture::wrapTex()
	{
		// Set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	void OpenGLTexture::setChannels(unsigned char* data)
	{
		// If there is txture data
		if (data)
		{
			// If there are 3 channels
			if (m_iChannels == 3)
			{
				glPixelStorei(GL_UNPACK_ALIGNMENT, 3); // Set storage mode for 3 channels
				// Define the texture image
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_iWidth, m_iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				glPixelStorei(GL_UNPACK_ALIGNMENT, 4); // Set storage mode for 4 channels
			}
			else if (m_iChannels == 4) // If there are 4 channels
			{
				// Define the texture image
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			}
			else if (m_iChannels == 1) // If there is 1 channel
			{
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Set storage mode for 1 channel
				// Define the texture image
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_iWidth, m_iHeight, 0, GL_RED, GL_UNSIGNED_BYTE, data);
				glPixelStorei(GL_UNPACK_ALIGNMENT, 4); // Set storage mode for 4 channels
			}
			else return; // If the number of channels is not 1, 3 or 4 then return

			// Generate a mipmap
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else return; // If no data then return
	}
}