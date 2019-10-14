#pragma once
#include "chpch.h"

#include "Charcoal/Events/Event.h"
#include "Charcoal/Core/Timestep.h"

namespace Charcoal
{
	class Layer
	{
	public:
		Layer(const std::string& name = "Layer") : m_DebugName(name) {}
		virtual ~Layer() {}

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep timestep) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline std::string GetDebugName() const { return m_DebugName; }

	private:
		std::string m_DebugName;
	};
}