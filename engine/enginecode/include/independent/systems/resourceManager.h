#pragma once

#include "system.h"
#include "core/assetManager.h"
#include "core/assetIncludes.h"

namespace Engine
{
	class ResourceManager : public System
	{
	private:
		static AssetManager<IndexBuffer> s_indexBuffers;
		static AssetManager<Shader> s_shaders;
		static AssetManager<Texture> s_textures;
		static AssetManager<VertexArray> s_VAOs;
		static AssetManager<VertexBuffer> s_VBOs;

		static std::string parseFilePath(const std::string& str);
	public:
		void start(SystemSignal init = SystemSignal::None, ...) override {}
		void stop(SystemSignal close = SystemSignal::None, ...) override;

		static std::shared_ptr<IndexBuffer> addIndexBuffer(const std::string& name, unsigned int* indices, unsigned int count);
		static std::shared_ptr<Shader> addShader(const std::string& filepath);
		static std::shared_ptr<Shader> addShader(const std::string& name, const std::string& vertexFilepath, const std::string& fragmentFilepath);
		static std::shared_ptr<Texture> addTexture(const std::string& filepath);
		static std::shared_ptr<Texture> addTexture(const std::string& name, unsigned int width, unsigned int height, unsigned int channels, unsigned char* texData);
		static std::shared_ptr<VertexArray> addVAO(const std::string& name);
		static std::shared_ptr<VertexBuffer> addVBO(const std::string& name, float* vertices, unsigned int size, BufferLayout& layout);
	};
}