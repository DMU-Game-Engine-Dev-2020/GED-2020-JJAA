/** \file renderAPI.h
*/
#pragma once

namespace Engine
{
	/**
	\class RenderAPI
	\brief Used to tell the render classes which API is being used

	So that they create the correct classes
	*/
	class RenderAPI
	{
	public:
		/**
		\class API
		\brief Enum for the API being used
		*/
		enum class API
		{
			None = 0, OpenGL = 1, Direct3D = 2
		};

		//! Used to get which API is being used
		/*!
		\return The API being used
		*/
		inline static API getAPI() { return ms_API; }
	private:
		static API ms_API; //!< Which API is being used
	};
}