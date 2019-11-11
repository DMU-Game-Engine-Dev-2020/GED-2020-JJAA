/** \file resourceManager.cpp
*/
#include "engine_pch.h"

#include "systems/resourceManager.h"

namespace Engine
{
	// Initialize the static asset managers
	AssetManager<IndexBuffer> ResourceManager::s_indexBuffers;
	AssetManager<Shader> ResourceManager::s_shaders;
	AssetManager<Texture> ResourceManager::s_textures;
	AssetManager<VertexArray> ResourceManager::s_VAOs;
	AssetManager<VertexBuffer> ResourceManager::s_VBOs;

	void ResourceManager::stop(SystemSignal close, ...)
	{
		// Call the clear functions for the asset managers
		s_indexBuffers.clear();
		s_shaders.clear();
		s_textures.clear();
		s_VAOs.clear();
		s_VBOs.clear();
	}

	std::shared_ptr<IndexBuffer> ResourceManager::addIndexBuffer(const std::string& name, unsigned int* indices, unsigned int count)
	{
		std::shared_ptr<IndexBuffer> temp; // Make a temporary shared pointer
		temp.reset(IndexBuffer::create(indices, count)); // Create an index buffer on the temporary pointer
		s_indexBuffers.add(name, temp); // Add the index buffer to the index buffer asset manager
		return temp; // Return the temporary pointer
	}

	std::shared_ptr<Shader> ResourceManager::addShader(const std::string& filepath)
	{
		std::shared_ptr<Shader> temp; // Make a temporary shared pointer
		temp.reset(Shader::create(filepath)); // Create a shader on the temporary pointer
		s_shaders.add(parseFilePath(filepath), temp); // Add the shader to the shader asset manager
		return temp; // Return the temporary pointer
	}

	std::shared_ptr<Shader> ResourceManager::addShader(const std::string& name, const std::string& vertexFilepath, const std::string& fragmentFilepath)
	{
		std::shared_ptr<Shader> temp; // Make a temporary shared pointer
		temp.reset(Shader::create(vertexFilepath, fragmentFilepath)); // Create a shader on the temporary pointer
		s_shaders.add(name, temp); // Add the shader to the shader asset manager
		return temp; // Return the temporary pointer
	}

	std::shared_ptr<Texture> ResourceManager::addTexture(const std::string& filepath)
	{
		std::shared_ptr<Texture> temp; // Make a temporary shared pointer
		temp.reset(Texture::createFromFile(filepath)); // Create a texture on the temporary pointer
		s_textures.add(parseFilePath(filepath), temp); // Add the texture to the texture asset manager
		return temp; // Return the temporary pointer
	}

	std::shared_ptr<Texture> ResourceManager::addTexture(const std::string& name, unsigned int width, unsigned int height, unsigned int channels, unsigned char* texData)
	{
		std::shared_ptr<Texture> temp; // Make a temporary shared pointer
		temp.reset(Texture::createFromRawData(width, height, channels, texData)); // Create a texture on the temporary pointer
		s_textures.add(name, temp); // Add the texture to the texture asset manager
		return temp; // Return the temporary pointer
	}

	std::shared_ptr<VertexArray> ResourceManager::addVAO(const std::string& name)
	{
		std::shared_ptr<VertexArray> temp; // Make a temporary shared pointer
		temp.reset(VertexArray::create()); // Create a vertex array on the temporary pointer
		s_VAOs.add(name, temp); // Add the vertex array to the vertex array asset manager
		return temp; // Return the temporary pointer
	}

	std::shared_ptr<VertexBuffer> ResourceManager::addVBO(const std::string& name, float* vertices, unsigned int size, BufferLayout& layout)
	{
		std::shared_ptr<VertexBuffer> temp; // Make a temporary shared pointer
		temp.reset(VertexBuffer::create(vertices, size, layout)); // Create a vertex buffer on the temporary pointer
		s_VBOs.add(name, temp); // Add the vertex buffer to the vertex buffer asset manager
		return temp; // Return the temporary pointer
	}

	std::string ResourceManager::parseFilePath(const std::string& str)
	{
		char sep = '/'; // Character that separates the file name from the rest of the file path

		int i = str.rfind(sep, str.length()); // Find the position in the string of the last occurence of sep
		if (i != std::string::npos) // If Sep is found
		{
			// Return a string with everything after the last sep in str
			return(str.substr(i + 1, str.length() - i));
		}
		else // If sep wasn't found
		{
			// Log an error
			LOG_ERROR("Problem finding name of file: {0}, using full filepath", str);
			return(str); // Return the full filepath
		}
	}
}