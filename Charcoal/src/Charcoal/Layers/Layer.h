#pragma once
#include "chpch.h"

#include "Charcoal/Events/Event.h"

namespace Charcoal
{
	class Layer
	{
	public:
		Layer(const std::string& name = "Layer") : m_DebugName(name) {}
		virtual ~Layer() {}

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event& event) {}

		inline std::string GetDebugName() const { return m_DebugName; }

	private:
		std::string m_DebugName;
	};
}