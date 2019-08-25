#pragma once

#include "Charcoal/Events/Event.h"
#include "Charcoal/Events/ApplicationEvent.h"
#include "Charcoal/Window.h"
#include "Charcoal/Log.h"

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

	private:
		bool OnWindowClose(WindowClosedEvent& event);

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running;
	};

}