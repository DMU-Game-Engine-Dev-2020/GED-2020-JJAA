/** \file resourceManager.cpp
*/
#include "engine_pch.h"

#include "systems/resourceManager.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace Engine
{
	// Initialize the static asset managers
	AssetManager<IndexBuffer> ResourceManager::s_indexBuffers;
	AssetManager<Shader> ResourceManager::s_shaders;
	AssetManager<Texture> ResourceManager::s_textures;
	AssetManager<VertexArray> ResourceManager::s_VAOs;
	AssetManager<VertexBuffer> ResourceManager::s_VBOs;
	AssetManager<Material> ResourceManager::s_materials;
	AssetManager<UniformBuffer> ResourceManager::s_UBOs;

	std::map<std::string, std::vector<Character>> ResourceManager::s_characters;
	std::shared_ptr<Texture> ResourceManager::s_fontTexture;

	void ResourceManager::stop(SystemSignal close, ...)
	{
		// Call the clear functions for the asset managers
		s_indexBuffers.clear();
		s_shaders.clear();
		s_textures.clear();
		s_VAOs.clear();
		s_VBOs.clear();
		s_materials.clear();
		s_UBOs.clear();
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
		const std::string name = parseFilePath(filepath);

		temp.reset(Shader::create(filepath)); // Create a shader on the temporary pointer
		s_shaders.add(name, temp); // Add the shader to the shader asset manager

		// Get any uniform buffer layouts from the shader just made
		UniformBufferLayouts buffers = temp->getUniformBufferLayouts();

		// If there is a uniform buffer
		if (!buffers.empty())
		{
			// For each element in the map
			for (auto& element : buffers)
			{
				// Add a new uniform buffer
				addUBO(element.first, element.second, name);
			}
		}

		return temp; // Return the temporary pointer
	}

	std::shared_ptr<Shader> ResourceManager::addShader(const std::string& name, const std::string& vertexFilepath, const std::string& fragmentFilepath)
	{
		std::shared_ptr<Shader> temp; // Make a temporary shared pointer
		temp.reset(Shader::create(vertexFilepath, fragmentFilepath)); // Create a shader on the temporary pointer
		s_shaders.add(name, temp); // Add the shader to the shader asset manager

		// Get any uniform buffer layouts from the shader just made
		UniformBufferLayouts buffers = temp->getUniformBufferLayouts();

		// If there is a uniform buffer
		if (!buffers.empty())
		{
			// For each element in the map
			for (auto& element : buffers)
			{
				// Add a new uniform buffer
				addUBO(element.first, element.second, name);
			}
		}

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

	std::shared_ptr<Material> ResourceManager::addMaterial(const std::string& name, std::shared_ptr<Shader> shader, std::shared_ptr<VertexArray> VAO)
	{
		std::shared_ptr<Material> temp; // Make a temporary shared pointer
		temp.reset(Material::create(shader, VAO)); // Create a material on the temporary pointer
		s_materials.add(name, temp); // Add the material to the material asset manager
		return temp; // Return the temporary pointer
	}

	std::shared_ptr<Material> ResourceManager::addMaterial(const std::string& name, std::shared_ptr<Shader> shader, std::shared_ptr<VertexBuffer> VBO)
	{
		std::shared_ptr<Material> temp; // Make a temporary shared pointer
		temp.reset(Material::create(shader, VBO)); // Create a material on the temporary pointer
		s_materials.add(name, temp); // Add the material to the material asset manager
		return temp; // Return the temporary pointer
	}

	std::shared_ptr<UniformBuffer> ResourceManager::addUBO(const std::string& name, std::shared_ptr<UniformBufferLayout> layout, const std::string& shaderName)
	{
		std::shared_ptr<UniformBuffer> temp; // Make a temporary shared pointer

		// If the uniform buffer doesn't already exist
		if (!s_UBOs.contains(name))
		{
			// Create a new buffer on the temporary pointer
			temp.reset(UniformBuffer::create(layout->getSize(), *layout));
			s_UBOs.add(name, temp); // Add the buffer to the uniform buffer asset manager
		}
		else // If the uniform buffer already exists
		{
			temp = s_UBOs.get(name); // Set the temporary pointer to the uniform buffer
		}
		temp->attachShaderBlock(s_shaders.get(shaderName), name); // Attach the shader block to the buffer
		return temp; // Return the temporary pointer
	}

	std::shared_ptr<UniformBuffer> ResourceManager::addUBO(const std::string& name, unsigned int rangeStart, unsigned int rangeEnd, std::shared_ptr<UniformBufferLayout> layout, const std::string& shaderName)
	{
		std::shared_ptr<UniformBuffer> temp; // Make a temporary shared pointer

		// If the uniform buffer doesn't already exist
		if (!s_UBOs.contains(name))
		{
			// Create a new buffer on the temporary pointer
			temp.reset(UniformBuffer::create(layout->getSize(), rangeStart, rangeEnd, *layout));
			s_UBOs.add(name, temp); // Add the buffer to the uniform buffer asset manager
		}
		else // If the uniform buffer already exists
		{
			temp = s_UBOs.get(name); // Set the temporary pointer to the uniform buffer
		}
		temp->attachShaderBlock(s_shaders.get(shaderName), name); // Attach the shader block to the buffer
		return temp; // Return the temporary pointer
	}

	void ResourceManager::populateCharacters(std::unordered_map<std::string, unsigned int> fontsAndSizes)
	{
		FT_Library ft;
		FT_Face face;

		if (FT_Init_FreeType(&ft))
			LOG_CRITICAL("Could not start FreeType");

		unsigned char* texMemory;
		int memH = 1024;
		int memW = 1024;
		int usedX = 0;
		int usedY = 0;

		texMemory = (unsigned char*)malloc(memW * memH);
		memset(texMemory, 0, memW * memH);

		std::map<std::string, std::pair<unsigned int, unsigned int>> maxSize;

		for (auto& element : fontsAndSizes)
		{
			if (FT_New_Face(ft, element.first.c_str(), 0, &face))
				LOG_CRITICAL("FreeType coudn't loat font: {0}", element.first);

			if (FT_Set_Pixel_Sizes(face, 0, element.second))
				LOG_CRITICAL("FreeType couldn't set font face size of {0}", element.second);

			maxSize.insert(std::make_pair(element.first, std::make_pair(0, 0)));

			for (int i = s_ASCIIStart; i <= s_ASCIIEnd; i++)
			{
				if (FT_Load_Char(face, i, FT_LOAD_RENDER))
					LOG_CRITICAL("Could not load the character {0}", i);

				maxSize[element.first].first = std::max(maxSize[element.first].first, face->glyph->bitmap.width);
				maxSize[element.first].second = std::max(maxSize[element.first].second, face->glyph->bitmap.rows);
			}
		}

		std::string thing;
		for (auto& element : fontsAndSizes)
		{
			if (FT_New_Face(ft, element.first.c_str(), 0, &face))
				LOG_CRITICAL("FreeType coudn't loat font: {0}", element.first);

			if (FT_Set_Pixel_Sizes(face, 0, element.second))
				LOG_CRITICAL("FreeType couldn't set font face size of {0}", element.second);

			std::vector<Character> characters;

			for (int i = s_ASCIIStart; i <= s_ASCIIEnd; i++)
			{
				if (FT_Load_Char(face, i, FT_LOAD_RENDER))
					LOG_CRITICAL("Could not load the character {0}", i);

				if (usedX + maxSize[element.first].first > memW)
				{
					usedY += maxSize[element.first].second;
					usedX = 0;
				}
				
				characters.push_back(Character(
					glm::vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
					glm::vec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
					face->glyph->advance.x
				));

				memset(texMemory, (int)face->glyph->bitmap.buffer, maxSize[element.first].first * maxSize[element.first].second);

				usedX += maxSize[element.first].first;
			}

			s_characters.insert(std::make_pair(parseFilePath(element.first), characters));
			thing = element.first;
		}

		usedY += maxSize[thing].second;

		memW = usedX;
		memH = usedY;

		unsigned char* texMemory2;
		texMemory2 = (unsigned char*)malloc(memW * memH);
		memset(texMemory2, 0, memW * memH);
		memcpy(texMemory2, texMemory, sizeof(texMemory2));

		for (auto element : s_characters)
			for (auto it = element.second.begin(); it != element.second.end(); it++)
				it->setUVs(glm::vec2((float)it->getSize().x / (float)memW, (float)it->getSize().y / (float)memH), 
					glm::vec2((float)it->getSize().x / (float)memW, (float)it->getSize().y / (float)memH));

		s_fontTexture.reset(Texture::createFromRawData(memW, memH, 1, texMemory2));

		free(texMemory);
		free(texMemory2);
	}

	std::shared_ptr<Character> ResourceManager::getCharacter(std::string font, unsigned int ASCIICode)
	{
		std::map<std::string, std::vector<Character>>::iterator it; // Make an iterator for the map
		it = s_characters.find(font); // Find the key in the map

		// If the key is not in the map
		if (it == s_characters.end())
		{
			// Log an error, cannot find key in the map
			LOG_ERROR("Font: '{0}' can't be found", font);
			return nullptr; // Return a null pointer
		}
		else // If the key is in the map
		{
			return std::make_shared<Character>(it->second[ASCIICode - s_ASCIIStart]); // Return the pointer
		}
	}

	std::string ResourceManager::parseFilePath(const std::string& str)
	{
		int i = str.rfind('/', str.length()); // Find the position in the string of the last occurence of '/'
		int j = str.rfind('.', str.length()); // Find the position in the string of the last occurence of '.'
		if (i != std::string::npos && j != std::string::npos) // If '/' and '.' are found
		{
			// Return a string with the name of the file
			return str.substr(i + 1, (str.length() - i) - (str.length() - (j - 1)));
		}
		else // If '/' and '.' were not found
		{
			// Log an error
			LOG_ERROR("Problem finding name of file: {0}, using full filepath", str);
			return str; // Return the full filepath
		}
	}
}