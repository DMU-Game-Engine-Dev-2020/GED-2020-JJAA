#pragma once

#include <glm/glm.hpp>

namespace Engine
{
	class Character
	{
	private:
		glm::vec2 m_startUV;
		glm::vec2 m_endUV;
		glm::ivec2 m_size;
		glm::ivec2 m_bearing;
		int m_iAdvance;
	public:
		Character() {}
		Character(glm::ivec2 size, glm::ivec2 bearing, int advance, glm::vec2 startUV = glm::vec2(0.f, 0.f), glm::vec2 endUV = glm::vec2(0.f, 0.f)) :
			m_startUV(startUV), m_endUV(endUV), m_size(size), m_bearing(bearing), m_iAdvance(advance) {}

		void setUVs(glm::vec2 startUV, glm::vec2 endUV) { m_startUV = startUV; m_endUV = endUV; }
		inline glm::vec2 getStartUV() { return m_startUV; }
		inline glm::vec2 getEndUV() { return m_endUV; }
		inline glm::ivec2 getSize() { return m_size; }
		inline glm::ivec2 getBearing() { return m_bearing; }
		inline int getAdvance() { return m_iAdvance; }
	};
}