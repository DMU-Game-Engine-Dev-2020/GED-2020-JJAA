#include "engine_pch.h"

#include "systems/resourceManager.h"

namespace Engine
{
	AssetManager<IndexBuffer> ResourceManager::s_indexBuffers;
	AssetManager<Shader> ResourceManager::s_shaders;
	AssetManager<Texture> ResourceManager::s_textures;
	AssetManager<VertexArray> ResourceManager::s_VAOs;
	AssetManager<VertexBuffer> ResourceManager::s_VBOs;

	void ResourceManager::stop(SystemSignal close, ...)
	{

	}

	std::shared_ptr<IndexBuffer> ResourceManager::addIndexBuffer(const std::string& name, unsigned int* indices, unsigned int count)
	{
		std::shared_ptr<IndexBuffer> temp;
		temp.reset(IndexBuffer::create(indices, count));
		s_indexBuffers.add(name, temp);
		return temp;
	}

	std::shared_ptr<Shader> ResourceManager::addShader(const std::string& filepath)
	{
		std::shared_ptr<Shader> temp;
		temp.reset(Shader::create(filepath));
		s_shaders.add(parseFilePath(filepath), temp);
		return temp;
	}

	std::shared_ptr<Shader> ResourceManager::addShader(const std::string& name, const std::string& vertexFilepath, const std::string& fragmentFilepath)
	{
		std::shared_ptr<Shader> temp;
		temp.reset(Shader::create(vertexFilepath, fragmentFilepath));
		s_shaders.add(name, temp);
		return temp;
	}

	std::shared_ptr<Texture> ResourceManager::addTexture(const std::string& filepath)
	{
		std::shared_ptr<Texture> temp;
		temp.reset(Texture::createFromFile(filepath));
		s_textures.add(parseFilePath(filepath), temp);
		return temp;
	}

	std::shared_ptr<Texture> ResourceManager::addTexture(const std::string& name, unsigned int width, unsigned int height, unsigned int channels, unsigned char* texData)
	{
		std::shared_ptr<Texture> temp;
		temp.reset(Texture::createFromRawData(width, height, channels, texData));
		s_textures.add(name, temp);
		return temp;
	}

	std::shared_ptr<VertexArray> ResourceManager::addVAO(const std::string& name)
	{
		std::shared_ptr<VertexArray> temp;
		temp.reset(VertexArray::create());
		s_VAOs.add(name, temp);
		return temp;
	}

	std::shared_ptr<VertexBuffer> ResourceManager::addVBO(const std::string& name, float* vertices, unsigned int size, BufferLayout& layout)
	{
		std::shared_ptr<VertexBuffer> temp;
		temp.reset(VertexBuffer::create(vertices, size, layout));
		s_VBOs.add(name, temp);
		return temp;
	}

	std::string ResourceManager::parseFilePath(const std::string& str)
	{
		char sep = '/';

		size_t i = str.rfind(sep, str.length());
		if (i != std::string::npos) 
		{
			return(str.substr(i + 1, str.length() - i));
		}

		return("");
	}
}