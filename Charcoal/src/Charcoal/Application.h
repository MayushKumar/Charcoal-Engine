#pragma once

#include "Charcoal/Core.h"

#include "Charcoal/Window.h"
#include "Charcoal/Events/Event.h"
#include "Charcoal/Events/ApplicationEvent.h"
#include "Charcoal/Layers/LayerStack.h"

#include "Charcoal/ImGui/ImGuiLayer.h"

namespace Charcoal {

	class Application
	{

	public:
		Application();
		virtual ~Application();

	public:
		void Run();
		void OnUpdate();
		void OnEvent(Event& event);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline Window& GetWindow() { return *m_Window; }

		static inline Application& GetApplication() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowClosedEvent& event);

	private:
		static Application* s_Instance;

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;

		bool m_Running;
		LayerStack m_LayerStack;
	};

}