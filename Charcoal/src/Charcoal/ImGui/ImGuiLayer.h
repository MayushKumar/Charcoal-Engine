#pragma once

#include "Charcoal/Layers/Layer.h"
#include "Charcoal/Events/Event.h"
#include "Charcoal/Events/ApplicationEvent.h"
#include "Charcoal/Events/MouseEvent.h"
#include "Charcoal/Events/KeyEvent.h"

namespace Charcoal
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		virtual ~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

	private:
		float m_Time = 0.0f;
	};
}