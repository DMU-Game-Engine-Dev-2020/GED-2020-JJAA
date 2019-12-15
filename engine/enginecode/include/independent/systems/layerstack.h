#pragma once

#include "systems/system.h"
#include "core/layer.h"

namespace Engine
{
	class Layerstack : public System
	{
	private:
		std::vector<std::shared_ptr<Layer>> m_layers;
	public:
		void start(SystemSignal init = SystemSignal::None, ...) override {} //!< A function to start the layerstack
		void stop(SystemSignal close = SystemSignal::None, ...) override {} //!< A function to stop the layerstack

		void push(std::shared_ptr<Layer> layer) { m_layers.push_back(layer); }
		void pop() { m_layers.pop_back(); }

		inline std::vector<std::shared_ptr<Layer>>::iterator begin() { return m_layers.begin(); }
		inline std::vector<std::shared_ptr<Layer>>::iterator end() { return m_layers.end(); }

		inline std::vector<std::shared_ptr<Layer>>::reverse_iterator rbegin() { return m_layers.rbegin(); }
		inline std::vector<std::shared_ptr<Layer>>::reverse_iterator rend() { return m_layers.rend(); }
	};
}