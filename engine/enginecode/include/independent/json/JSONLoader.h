#pragma once

#include <fstream>

#include "json.hpp"
#include "JSONlayer.h"
#include "rendering/text/textLabel.h"
#include "cameras/freeOrthoCameraController2D.h"
#include "cameras/FPSCameraControllerEuler.h"

#include "systems/log.h"

// Temporary until we introduce ASSIMP
#include "textLoader.h"

namespace Engine
{
	class JSONLoader
	{
	private:
	public:
		static void load(const std::string& filepath, JSONLayer& layer)
		{
			std::fstream handle(filepath, std::ios::in);
			if (!handle.is_open()) {
				LOG_CRITICAL("Could not open file: {0}", filepath);
			}
			nlohmann::json layerJSON;
			handle >> layerJSON;

			if (layerJSON.count("Asyncload") > 0)
			{
				if (layerJSON["Asyncload"].count("shaders") > 0)
				{
					for (auto& filepath : layerJSON["Asyncload"]["shaders"])
					{
						if (filepath.count("filepath") > 0) layer.getResources()->addShader(filepath["filepath"].get<std::string>());
					}
				}
				if (layerJSON["Asyncload"].count("textures") > 0)
				{
					for (auto& filepath : layerJSON["Asyncload"]["textures"])
					{
						if (filepath.count("filepath") > 0) layer.getResources()->addTexture(filepath["filepath"].get<std::string>());
					}
				}
				if (layerJSON["Asyncload"].count("fonts") > 0)
				{
					std::unordered_map<std::string, unsigned int> fonts;
					for (auto& filepath : layerJSON["Asyncload"]["fonts"])
					{
						fonts[filepath["filepath"].get<std::string>()] = filepath["charSize"].get<int>();
					}
					if (!fonts.empty()) layer.getResources()->populateCharacters(fonts);
				}
			}

			if (layerJSON.count("camera") > 0)
			{
				std::string type = layerJSON["camera"]["type"].get<std::string>();
				if (type.compare("Euler3D") == 0)
				{
					//layer.getCamera().reset(new FPSCameraControllerEuler);
					layer.m_pCamera.reset(new FPSCameraControllerEuler);
					float fov = layerJSON["camera"]["fov"].get<float>();
					float aspectRatio = layerJSON["camera"]["aspectRatio"].get<float>();
					float nearClip = layerJSON["camera"]["nearClip"].get<float>();
					float farClip = layerJSON["camera"]["farClip"].get<float>();
					layer.getCamera()->init(fov, aspectRatio, nearClip, farClip);
				}
				else if (type.compare("FreeOrtho2D") == 0)
				{
					layer.getCamera().reset(new FreeOrthoCameraController2D);
					float top = layerJSON["camera"]["top"].get<float>();
					float left = layerJSON["camera"]["left"].get<float>();
					float width = layerJSON["camera"]["width"].get<float>();
					float height = layerJSON["camera"]["height"].get<float>();
					layer.getCamera()->init(left, top, width, height);
				}
			}

			if (layerJSON.count("renderer") > 0)
			{
				std::string type = layerJSON["renderer"]["type"].get<std::string>();
				if (type.compare("Basic3D") == 0)
				{
					//layer.getRenderer().reset(Renderer::createBasic3D());
					layer.m_pRenderer.reset(Renderer::createBasic3D());
				}
				if (type.compare("BasicText2D") == 0) layer.getRenderer().reset(Renderer::createText());
			}

			if (layerJSON.count("MemoryInfo") > 0)
			{
				layer.getGameObjects().resize(layerJSON["MemoryInfo"]["gameObjects"].get<int>());
				layer.getMaterials().resize(layerJSON["MemoryInfo"]["materials"].get<int>());
				layer.getPositions().resize(layerJSON["MemoryInfo"]["position"].get<int>());
				layer.getVelocities().resize(layerJSON["MemoryInfo"]["velocity"].get<int>());
				layer.getTextures().resize(layerJSON["MemoryInfo"]["texture"].get<int>());
				layer.getControllers().resize(layerJSON["MemoryInfo"]["controllers"].get<int>());
			}


			if (layerJSON.count("GameObjects") > 0)
			{
				int goIndex = 0;
				int materialsIndex = 0;
				int positionsIndex = 0;
				int velocitiesIndex = 0;
				int texturesIndex = 0;
				int controllersIndex = 0;
				for (auto& object : layerJSON["GameObjects"])
				{
					// Create Game Object
					std::string name = "Name";
					if (object.count("name") > 0) name = object["name"].get<std::string>();
					layer.getGameObjects().at(goIndex) = (std::make_shared<GameObject>(GameObject(name)));
					auto gameObject = layer.getGameObjects().at(goIndex);
					goIndex++;


					// Check which components need adding
					if (object.count("material") > 0) {
						if (object["material"].count("model") > 0)
						{
							TextModel model;
							if (!TextLoader::loadModel(layer.getResources(), object["material"]["model"], model)) exit(0);
							std::shared_ptr<VertexArray> VAO = layer.getResources()->addVAO(name);
							VAO->setVertexBuffer(layer.getResources()->addVBO(name, model.vertices, sizeof(float) * model.verticesSize, model.shader->getBufferLayout()));
							VAO->setIndexBuffer(layer.getResources()->addIndexBuffer(name, model.indices, model.indicesSize));
							std::shared_ptr<Material> mat = layer.getResources()->addMaterial(name, model.shader, VAO);
							//if (model.texture != nullptr)
							//{
							//	layer.getData().push_back((void *)new int(model.texture->getSlot()));
							//	mat->setDataElement("u_texData", (void*)layer.getData().back());
							//}
							layer.getMaterials().at(materialsIndex) = std::make_shared<MaterialComponent>(MaterialComponent(mat));
							gameObject->addComponent(layer.getMaterials().at(materialsIndex));
							materialsIndex++;
						}
						else if (object["material"].count("text") > 0)
						{
							std::string text = object["material"]["text"].get<std::string>();
							std::string font = object["material"]["font"].get<std::string>();
							int charSize = object["material"]["charSize"].get<int>();
							float r = object["material"]["colour"]["r"].get<float>();
							float g = object["material"]["colour"]["g"].get<float>();
							float b = object["material"]["colour"]["b"].get<float>();
							std::shared_ptr<TextLabel> label(TextLabel::create(font, charSize, text, glm::vec2(), 0.f, 0.f, glm::vec3(r, g, b)));
							auto& mat = label->getMaterial();
							layer.getData().push_back((void *) new int(layer.getResources()->getFontTexture()->getSlot()));
							mat->setDataElement("u_texData", (void*)layer.getData().back());
							layer.getData().push_back((void *)new glm::vec3(r, g, b));
							mat->setDataElement("u_fontColour", (void*)&(*(glm::vec3*)layer.getData().back())[0]);

							layer.getMaterials().at(materialsIndex) = std::make_shared<MaterialComponent>(MaterialComponent(mat));
							gameObject->addComponent(layer.getMaterials().at(materialsIndex));
							materialsIndex++;
						}
					}
					if (object.count("position") > 0) {
						glm::vec3 translation(object["position"]["trans"]["x"].get<float>(), object["position"]["trans"]["y"].get<float>(), object["position"]["trans"]["z"].get<float>());
						glm::vec3 rotation(object["position"]["rot"]["x"].get<float>(), object["position"]["rot"]["y"].get<float>(), object["position"]["rot"]["z"].get<float>());
						glm::vec3 scale(object["position"]["scale"]["x"].get<float>(), object["position"]["scale"]["y"].get<float>(), object["position"]["scale"]["z"].get<float>());
						layer.getPositions().at(positionsIndex) = std::make_shared<PositionComponent>(PositionComponent(translation, rotation, scale));
						gameObject->addComponent(layer.getPositions().at(positionsIndex));
						positionsIndex++;
					}
					if (object.count("velocity") > 0) {
						glm::vec3 linear(object["velocity"]["linear"]["x"].get<float>(), object["velocity"]["linear"]["y"].get<float>(), object["velocity"]["linear"]["z"].get<float>());
						glm::vec3 angular(object["velocity"]["angular"]["x"].get<float>(), object["velocity"]["angular"]["y"].get<float>(), object["velocity"]["angular"]["z"].get<float>());
						layer.getVelocities().at(velocitiesIndex) = std::make_shared<VelocityComponent>(VelocityComponent(linear, angular));
						gameObject->addComponent(layer.getVelocities().at(velocitiesIndex));
						velocitiesIndex++;
					}
					if (object.count("texture") > 0) {
						std::shared_ptr<Texture> tex = layer.getResources()->addTexture(object["texture"]["tex"].get<std::string>());
						layer.getTextures().at(texturesIndex) = std::make_shared<TextureComponent>(TextureComponent(tex));
						gameObject->addComponent(layer.getTextures().at(texturesIndex));
						texturesIndex++;
					}
					if (object.count("textureSwitch") > 0) {
						std::shared_ptr<Texture> tex1 = layer.getResources()->addTexture(object["textureSwitch"]["tex1"].get<std::string>());
						std::shared_ptr<Texture> tex2 = layer.getResources()->addTexture(object["textureSwitch"]["tex2"].get<std::string>());
						layer.getControllers().at(controllersIndex) = std::make_shared<TextureSwitchComponent>(TextureSwitchComponent(tex1, tex2));
						gameObject->addComponent(layer.getControllers().at(controllersIndex));
						controllersIndex++;
					}
					if (object.count("oscillate") > 0) {
						OscilateComponent::state state;
						auto stateStr = object["oscillate"]["state"].get<std::string>();
						if (stateStr.compare("down") == 0) state = OscilateComponent::state::DOWN;
						if (stateStr.compare("stopped") == 0) state = OscilateComponent::state::STOPPED;
						if (stateStr.compare("up") == 0) state = OscilateComponent::state::UP;
						float time = object["oscillate"]["time"].get<float>();
						layer.getControllers().at(controllersIndex) = std::make_shared<OscilateComponent>(OscilateComponent(state, time));
						gameObject->addComponent(layer.getControllers().at(controllersIndex));
						controllersIndex++;
					}
					if (object.count("rotate") > 0) {
						layer.getControllers().at(controllersIndex) = std::make_shared<RotateComponent>(RotateComponent());
						gameObject->addComponent(layer.getControllers().at(controllersIndex));
						controllersIndex++;
					};
				}
			}
			else LOG_INFO("None");

			if (layerJSON.count("UBOs") > 0)
			{
				for (auto& object : layerJSON["UBOs"])
				{
					std::string uboName = object["name"].get<std::string>();
					std::shared_ptr<UniformBuffer> UBO = layer.getResources()->getUBO(uboName);
					std::vector<void*> uboData;
					if (object.count("data") > 0)
					{
						for (auto& object2 : object["data"])
						{
							void* ptr = nullptr;
							auto& type = object2["type"];
							if (type == "pointer")
							{
								if (object2["var"].get<std::string>().compare("CAM_VIEW") == 0) ptr = (void *)&layer.getCamera()->getCamera()->getView();
								if (object2["var"].get<std::string>().compare("CAM_PROJ") == 0) ptr = (void *)&layer.getCamera()->getCamera()->getProjection();
								if (object2["var"].get<std::string>().compare("CAM_VIEWPROJ") == 0) ptr = (void *)&layer.getCamera()->getCamera()->getViewProjection();
								if (object2["var"].get<std::string>().compare("CAM_POS") == 0) ptr = (void *)&layer.getCamera()->getCamera()->getPosition();
							}
							if (type == "Float3")
							{
								layer.getData().push_back(new glm::vec4(object2["x"].get<float>(), object2["y"].get<float>(), object2["z"].get<float>(), 1.0f));
								ptr = (void*)&(*(glm::vec4*)layer.getData().back())[0];
							}
							uboData.push_back(ptr);
						}
					}
					(*layer.getSceneData())[UBO] = uboData;
				}
			}

			if (layerJSON.count("RendererCommands") > 0)
			{
				std::string stages[4] = { "init", "predraw", "postdraw", "exit" };
				for (int i = 0; i < 4; i++)
				{
					if (layerJSON["RendererCommands"].count(stages[i]) > 0)
					{
						for (auto& object : layerJSON["RendererCommands"][stages[i]])
						{
							std::string type = object["type"].get<std::string>();

							std::shared_ptr<RenderCommand> command;

							bool keepAlive = true; // Adding to shared ptrs to leave deletion to the smart ptr

							if (type.compare("ClearDepthColourBuffer") == 0)
							{
								command.reset(RenderCommand::clearDepthColourBufferCommand(keepAlive));
							}

							if (type.compare("SetClearColour") == 0)
							{
								float r = object["r"].get<float>();
								float g = object["g"].get<float>();
								float b = object["b"].get<float>();
								float a = object["a"].get<float>();
								command.reset(RenderCommand::setClearColourCommand(r, g, b, a, keepAlive));
							}

							if (type.compare("SetDepthTestLess") == 0)
							{
								bool enabled = object["enabled"].get<bool>();
								command.reset(RenderCommand::setDepthTestLessCommand(enabled, keepAlive));
							}

							if (type.compare("SetBackfaceCulling") == 0)
							{
								bool enabled = object["enabled"].get<bool>();
								command.reset(RenderCommand::setBackfaceCullingCommand(enabled, keepAlive));
							}

							if (type.compare("SetOneMinusAlphaBlending") == 0)
							{
								bool enabled = object["enabled"].get<bool>();
								command.reset(RenderCommand::setOneMinusAlphaBlendingCommand(enabled, keepAlive));
							}

							if (i == 0) layer.getInitCommands().push_back(command);
							if (i == 1) layer.getPredrawCommands().push_back(command);
							if (i == 2) layer.getPostdrawCommands().push_back(command);
							if (i == 3) layer.getExitCommands().push_back(command);
						}
					}
				}
			}
		}
	};
}