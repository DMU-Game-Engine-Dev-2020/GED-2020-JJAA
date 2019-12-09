#pragma once

#include "textLabel.h"

namespace Engine
{
	class BasicTextLabel : public TextLabel
	{
	private:
		std::string m_font;
		unsigned int m_iSize;
		std::string m_text;
		glm::vec2 m_position;
		float m_fAngle;
		float m_fScale;
		glm::vec3 m_colour;

		glm::vec4 m_model;
		std::shared_ptr<Material> m_material;
	public:
		BasicTextLabel(const std::string& fontName, unsigned int charSize, const std::string& text,
			const glm::vec2& position, float orientation, float scale, const glm::vec3& colour);

		bool setFont(const std::string& fontName) override;
		bool setCharSize(unsigned int charSize) override;
		void setText(const std::string& text) override;
		void setPosition(const glm::vec2& position) override;
		void setOrientation(float angle) override;
		void setScale(float scale) override;
		void setColour(const glm::vec3& colour) override;

		inline const std::string& getFont() const override { return m_font; }
		inline unsigned int getCharSize() const override { return m_iSize; }
		inline const std::string& getText() const override { return m_text; }
		inline glm::vec2 getPosition() const override { return m_position; }
		inline float getOrientation() const override { return m_fAngle; }
		inline float getScale() const override { return m_fScale; }
		inline glm::vec3 getColour() const override { return m_colour; }
		inline glm::vec4 getModel() const override { return m_model; }
		inline std::shared_ptr<Material> getMaterial() const override { return m_material; }
	};
}