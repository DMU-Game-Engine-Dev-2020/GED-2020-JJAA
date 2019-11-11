/** \file OpenGLTexture.h
*/
#pragma once

#include "rendering/texture.h"

#include <list>

namespace Engine
{
	/**
	\class OpenGLTexture
	\brief OpenGL specific texture
	*/
	class OpenGLTexture : public Texture
	{
	private:
		unsigned int m_iTextureID; //!< The texture ID

		unsigned int m_iWidth; //!< The width of the texture
		unsigned int m_iHeight; //!< The height of the texture
		unsigned int m_iChannels; //!< The textures channels
		unsigned int m_iTextureSlot; //!< The texture slot for this texture

		static unsigned int s_iTextureSlot; //!< The next texture slot to use
		static std::list<unsigned int> s_deletedSlots; //!< List of slots which were used but are no longer being used so can be used again

		//! Function to create the texture
		void genTex();
		//! Function to set the texture wrapping
		void wrapTex();
		//! Function to set the channels for the texture
		/*!
		\param data The texture data
		*/
		void setChannels(unsigned char* data);
	public:
		//! Constructor
		/*!
		\param filepath The filepath to where the texture is saved
		*/
		OpenGLTexture(const std::string& filepath);
		//! Constructor
		/*!
		\param width The width of the texture
		\param height The height of the texure
		\param channels The number of channels
		\param texData The texture data
		*/
		OpenGLTexture(unsigned int width, unsigned int height, unsigned int channels, unsigned char* texData);
		//! Destructor
		~OpenGLTexture();

		//! Function to get the width
		/*!
		\return The width of the texture
		*/
		inline unsigned int getWidth() const override { return m_iWidth; }
		//! Function to get the height
		/*!
		\return The height of the texture
		*/
		inline unsigned int getHeight() const override { return m_iHeight; }
		//! Function to get the channels of the texture
		/*!
		\return The channels of the texture
		*/
		inline unsigned int getChannels() const override { return m_iChannels; }
		//! Function to get the texture slot
		/*!
		\return The texture slot
		*/
		inline unsigned int getSlot() const override { return m_iTextureSlot; }
	};
}