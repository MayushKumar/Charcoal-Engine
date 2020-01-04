#pragma once

#include "Charcoal/Core/Core.h"

#include "Charcoal/Core/Window.h"
#include "Charcoal/Events/Event.h"
#include "Charcoal/Events/ApplicationEvent.h"
#include "Charcoal/Layers/LayerStack.h"
#include "Charcoal/Core/Timestep.h"

#include "Charcoal/ImGui/ImGuiLayer.h"

namespace Charcoal {

	class Application
	{

	public:
		Application();
		virtual ~Application();

	public:
		void Run();
		void ShutDown();
		void OnEvent(Event& event);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline Window& GetWindow() { return *m_Window; }

		static inline Application& GetApplication() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowClosedEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		static Application* s_Instance;

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;

		bool m_Running;
		bool m_Minimized;
		LayerStack m_LayerStack;

		float m_LastFrameTime;
	};

}