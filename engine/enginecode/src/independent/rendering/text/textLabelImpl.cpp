#include "engine_pch.h"

#include "rendering/text/textLabelImpl.h"

namespace Engine
{
	TextLabelImpl::TextLabelImpl(const std::string& fontName, unsigned int charSize, const std::string& text, 
		const glm::vec2& position, float orientation, float scale, const glm::vec3& colour)
	{
		m_font = fontName;
		m_iSize = charSize;
		m_text = text;
		m_position = position;
		m_fAngle = orientation;
		m_fScale = scale;
		m_colour = colour;

		//std::shared_ptr<ResourceManager> resources;

		/*float textVertices[4 * 4] = {
			0,              0,              ch.getStartUV().x, ch.getStartUV().y,
			ch.getSize().x, 0,              ch.getEndUV().x,   ch.getStartUV().y,
			ch.getSize().x, ch.getSize().y, ch.getEndUV().x,   ch.getEndUV().y,
			0,              ch.getSize().y, ch.getStartUV().x, ch.getEndUV().y
		};

		unsigned int textIndices[4] = { 0, 1, 2, 3 };

		std::shared_ptr<VertexArray> tempSetupVAO;
		std::shared_ptr<Shader>tempSetupShader;

		tempSetupVAO = ResourceManager::addVAO(text);
		tempSetupVAO->setVertexBuffer(ResourceManager::addVBO(text, textVertices, sizeof(textVertices), tempSetupShader->getBufferLayout()));
		tempSetupVAO->setIndexBuffer(ResourceManager::addIndexBuffer(text, textIndices, 4));

		m_material = ResourceManager::addMaterial(text, tempSetupShader, tempSetupVAO);*/
	}

	bool TextLabelImpl::setFont(const std::string& fontName)
	{
		return false;
	}

	bool TextLabelImpl::setCharSize(unsigned int charSize)
	{
		return false;
	}

	void TextLabelImpl::setText(const std::string& text)
	{

	}

	void TextLabelImpl::setPosition(const glm::vec2& position)
	{

	}

	void TextLabelImpl::setOrientation(float angle)
	{

	}

	void TextLabelImpl::setScale(float scale)
	{

	}

	void TextLabelImpl::setColour(const glm::vec3& colour)
	{

	}
}