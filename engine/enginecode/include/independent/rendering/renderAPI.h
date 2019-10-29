/** \file renderAPI.h
*/
#pragma once

namespace Engine
{
	class RenderAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1, Direct3D = 2
		};

		inline static API getAPI() { return ms_API; }
	private:
		static API ms_API;
	};
}