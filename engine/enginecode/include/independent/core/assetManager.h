#pragma once

#include "systems/log.h"

#include <map>

namespace Engine
{
	template <class G>
	class AssetManager
	{
	private:
		std::map<std::string, std::shared_ptr<G>> m_container;
	public:
		bool contains(const std::string& key);
		void add(const std::string& key, std::shared_ptr<G>& element);
		std::shared_ptr<G> get(const std::string& key);
	};

	template<class G>
	bool AssetManager<G>::contains(const std::string& key)
	{
		std::map<std::string, std::shared_ptr<G>>::iterator it;
		it = m_container.find(key);

		if (it == m_container.end())
			return false;
		else
			return true;
	}

	template<class G>
	void AssetManager<G>::add(const std::string& key, std::shared_ptr<G>& element)
	{
		std::map<std::string, std::shared_ptr<G>>::iterator it;
		it = m_container.find(key);

		if (it != m_container.end())
			LOG_ERROR("Asset with key: '{0}' already exists, use a different key", key);
		else
		{
			m_container.insert(std::make_pair(key, element));
		}
	}

	template<class G>
	inline std::shared_ptr<G> AssetManager<G>::get(const std::string& key)
	{
		std::map<std::string, std::shared_ptr<G>>::iterator it;
		it = m_container.find(key);

		if (it == m_container.end())
		{
			LOG_ERROR("Asset with key: '{0}' doesn't exist, use a different key", key);
			return std::shared_ptr<G>;
		}
		else
		{
			return it;
		}
	}
}