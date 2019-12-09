#include "engine_pch.h"

#include "rendering/text/basicTextLabel.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
	BasicTextLabel::BasicTextLabel(const std::string& fontName, unsigned int charSize, const std::string& text, 
		const glm::vec2& position, float orientation, float scale, const glm::vec3& colour)
	{
		m_font = fontName;
		m_iSize = charSize;
		m_text = text;
		m_position = position;
		m_fAngle = orientation;
		m_fScale = scale;
		m_colour = colour;

		std::vector<float*> textVertices;
		textVertices.resize(text.size());

		/*int i = 0;
		for (auto& it : textVertices)
		{
			std::shared_ptr<Character> ch = ResourceManager::getCharacter(fontName, int(text.at(i)));

			float vertices[4 * 4] = {
			0,               0,               ch->getStartUV().x, ch->getStartUV().y,
			ch->getSize().x, 0,               ch->getEndUV().x,   ch->getStartUV().y,
			ch->getSize().x, ch->getSize().y, ch->getEndUV().x,   ch->getEndUV().y,
			0,               ch->getSize().y, ch->getStartUV().x, ch->getEndUV().y
			};

			it = vertices;
			i++;
		}*/

		std::shared_ptr<Character> ch = ResourceManager::getCharacter(fontName, int(text.at(0)));

		float vertices[4 * 4] = {
		0,               0,               ch->getStartUV().x, ch->getStartUV().y,
		ch->getSize().x, 0,               ch->getEndUV().x,   ch->getStartUV().y,
		ch->getSize().x, ch->getSize().y, ch->getEndUV().x,   ch->getEndUV().y,
		0,               ch->getSize().y, ch->getStartUV().x, ch->getEndUV().y
		};

		unsigned int textIndices[4] = { 0, 1, 2, 3 };

		std::shared_ptr<Shader> tempSetupShader = ResourceManager::addShader("assets/shaders/text2.glsl");
		std::shared_ptr<VertexArray> tempSetupVAO = ResourceManager::addVAO(text);
		tempSetupVAO->setVertexBuffer(ResourceManager::addVBO(text, vertices, sizeof(vertices), tempSetupShader->getBufferLayout()));
		tempSetupVAO->setIndexBuffer(ResourceManager::addIndexBuffer(text, textIndices, 4));

		m_material = ResourceManager::addMaterial(text, tempSetupShader, tempSetupVAO);


		glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(m_position, 0));
		glm::mat4 rotate = glm::rotate(glm::mat4(1.f), glm::radians(m_fAngle), glm::vec3(0.f, 0.f, 1.f));
		glm::mat4 scaled = glm::scale(glm::mat4(1.f), glm::vec3(glm::vec2(m_fScale), 1));
		m_model = translate * rotate * scaled;
	}

	bool BasicTextLabel::setFont(const std::string& fontName)
	{
		m_font = fontName;

		return false;
	}

	bool BasicTextLabel::setCharSize(unsigned int charSize)
	{
		return false;
	}

	void BasicTextLabel::setText(const std::string& text)
	{

	}

	void BasicTextLabel::setPosition(const glm::vec2& position)
	{

	}

	void BasicTextLabel::setOrientation(float angle)
	{

	}

	void BasicTextLabel::setScale(float scale)
	{

	}

	void BasicTextLabel::setColour(const glm::vec3& colour)
	{

	}
}