/** \file resourceManager.h
*/
#pragma once

#include "system.h"
#include "core/assetManager.h"
#include "core/assetIncludes.h"

namespace Engine
{
	/**
	\class ResourceManager
	\brief Manages all asset managers
	*/
	class ResourceManager : public System
	{
	private:
		static AssetManager<IndexBuffer> s_indexBuffers; //!< Index buffer asset manager
		static AssetManager<Shader> s_shaders; //!< Shader asset manager
		static AssetManager<Texture> s_textures; //!< Texture asset manager
		static AssetManager<VertexArray> s_VAOs; //!< Vertex array asset manager
		static AssetManager<VertexBuffer> s_VBOs; //!< Vertex buffer asset manager
		static AssetManager<Material> s_materials;
		static AssetManager<UniformBuffer> s_UBOs;

		//! Function to get the name of a file from a file path
		/*!
		\param str The file path
		\return The name of the file
		*/
		static std::string parseFilePath(const std::string& str);
	public:
		ResourceManager() {} //!< Constructor

		//! Function to start the manager
		void start(SystemSignal init = SystemSignal::None, ...) override {}
		//! Function to stop the manager
		void stop(SystemSignal close = SystemSignal::None, ...) override;

		//! Function to add an index buffer
		/*!
		\param name The name of the buffer
		\param indices The indices of the thing being drawn
		\param count The number of indices
		\return A pointer to the buffer
		*/
		static std::shared_ptr<IndexBuffer> addIndexBuffer(const std::string& name, unsigned int* indices, unsigned int count);
		//! Function to add a shader
		/*!
		\param filepath The file path for where the shader file is
		\return A pointer to the shader
		*/
		static std::shared_ptr<Shader> addShader(const std::string& filepath);
		//! Function to add a shader
		/*!
		\param name The name of the buffer
		\param vertexFilepath The file path for where the vertex shader file is
		\param fragmentFilepath The file path for where the fragment shader file is
		\return A pointer to the shader
		*/
		static std::shared_ptr<Shader> addShader(const std::string& name, const std::string& vertexFilepath, const std::string& fragmentFilepath);
		//! Function to add a texture
		/*!
		\param filepath The file path for where the texture is
		\return A pointer to the texture
		*/
		static std::shared_ptr<Texture> addTexture(const std::string& filepath);
		//! Function to add a texture
		/*!
		\param name The name of the shader
		\param width The width of the texture
		\param height The height of the texure
		\param channels The number of channels
		\param texData The texture data
		\return A pointer to the texture
		*/
		static std::shared_ptr<Texture> addTexture(const std::string& name, unsigned int width, unsigned int height, unsigned int channels, unsigned char* texData);
		//! Function to add a vertex array
		/*!
		\param name The name of the array
		\return A pointer to the array
		*/
		static std::shared_ptr<VertexArray> addVAO(const std::string& name);
		//! Function to add a vertex buffer
		/*!
		\param name The name of the buffer
		\param vertices The number of vertices
		\param size The size in memory of the vertices
		\param layout The buffer layout
		\return A pointer to the buffer
		*/
		static std::shared_ptr<VertexBuffer> addVBO(const std::string& name, float* vertices, unsigned int size, BufferLayout& layout);

		static std::shared_ptr<Material> addMaterial(const std::string& name, std::shared_ptr<Shader> shader, std::shared_ptr<VertexArray> VAO);
		static std::shared_ptr<Material> addMaterial(const std::string& name, std::shared_ptr<Shader> shader, std::shared_ptr<VertexBuffer> VBO);

		static std::shared_ptr<UniformBuffer> addUniformBuffer(const std::string& name, unsigned int size, UniformBufferLayout& layout, const std::string& shaderName);

		static std::list<std::shared_ptr<UniformBuffer>> getUniformBuffers();
	};
}