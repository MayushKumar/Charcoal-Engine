#include <chpch.h>

#include "Charcoal/Core/Application.h"

#include "Charcoal/Debug/Instrumentor.h"

#include "Charcoal/Input/Input.h"
#include "Charcoal/Input/KeyCodes.h"

#include "Charcoal/Renderer/RendererCommand.h"
#include "Charcoal/Renderer/Renderer3D.h"
#include "Charcoal/Renderer/Renderer2D.h"

#include <GLFW/glfw3.h>
#include <imgui.h>

namespace Charcoal {

	Application* Application::s_Instance = nullptr;

	Application::Application() : m_Running(true), m_Minimized(false), m_LayerStack()
	{
		CH_PROFILE_BEGIN_SESSION("CharcoalProfile-Startup", "CharcoalProfile-Startup.json");
		CH_PROFILE_FUNCTION();

		CH_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(CH_BIND_EVENT_FUNC(Application::OnEvent));

		RendererCommand::Init();
		Renderer3D::Init();
		Renderer2D::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		CH_PROFILE_FUNCTION();

		CH_PROFILE_END_SESSION();
	}

	void Application::Run()
	{
		CH_PROFILE_END_SESSION();
		CH_PROFILE_BEGIN_SESSION("CharcoalProfile-Runtime", "CharcoalProfile-Runtime.json");

		while (m_Running)
		{
			CH_PROFILE_SCOPE("Run Loop");

			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;
			
			if (!m_Minimized)
			{
				for (Layer* layer : m_LayerStack)
				{
					layer->OnUpdate(timestep);
					RendererCommand::ClearDepthBuffer();
				}
			}

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}

		CH_PROFILE_END_SESSION();
		CH_PROFILE_BEGIN_SESSION("CharcoalProfile-Shutdown", "CharcoalProfile-Shutdown.json");

		ShutDown();
	}

	void Application::ShutDown()
	{
		CH_PROFILE_FUNCTION();

		Renderer2D::ShutDown();
		Renderer3D::ShutDown();
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowResizeEvent>(CH_BIND_EVENT_FUNC(Application::OnWindowResize));
		dispatcher.Dispatch<WindowClosedEvent>(CH_BIND_EVENT_FUNC(Application::OnWindowClose));

		ImGuiIO& imguiIO = ImGui::GetIO();
		
		if (!m_Minimized)
		{
			if(((event.GetCategoryFlags() | EventCategory::MouseInputEvent) && imguiIO.WantCaptureMouse) ||
			   ((event.GetCategoryFlags() | EventCategory::KeyInputEvent) && imguiIO.WantCaptureKeyboard)) return;
		
			for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
			{
				(*--it)->OnEvent(event);
				if (event.Handled())
					break;
			}
		}
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetHeight() == 0 || e.GetWidth() == 0)
		{
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;
		RendererCommand::SetViewport(0, 0, e.GetWidth(), e.GetHeight());
		return false;
	}

	bool Application::OnWindowClose(WindowClosedEvent& e)
	{
		m_Running = false;
		return true;
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

}
