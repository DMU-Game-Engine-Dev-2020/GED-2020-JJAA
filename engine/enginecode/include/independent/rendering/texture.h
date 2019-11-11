/** \file texture.h
*/
#pragma once

#include <string>

namespace Engine
{
	/**
	\class Texture
	\brief A base interface class for API specific texture classes to use
	*/
	class Texture
	{
	public:
		//! Function to get the width of the texture
		/*!
		\return The width of the texture
		*/
		virtual unsigned int getWidth() const = 0;
		//! Function to get the height of the texture
		/*!
		\return The height of the texture
		*/
		virtual unsigned int getHeight() const = 0;
		//! Function to get the channels of the texture
		/*!
		\return The channels of the texture
		*/
		virtual unsigned int getChannels() const = 0;
		//! Function to get the texture slot
		/*!
		\return The texture slot
		*/
		virtual unsigned int getSlot() const = 0;

		//! Function to create the texture from a file
		/*!
		\param filepath The filepath for where the texture is saved
		\return A pointer to the texture
		*/
		static Texture* createFromFile(const std::string& filepath);
		//! Function to create the texture from data
		/*!
		\param width The width of the texture
		\param height The height of the texure
		\param channels The number of channels
		\param texData The texture data
		\return A pointer to the texture
		*/
		static Texture* createFromRawData(unsigned int width, unsigned int height, unsigned int channels, unsigned char* texData);
	};
}