#include <chpch.h>

#include "Core.h"
#include "Application.h"

#include "Input/Input.h"
#include "Input/KeyCodes.h"

#include "Renderer/RendererCommand.h"
#include "Renderer/Renderer.h"

//#include <glad/glad.h>

namespace Charcoal {

#define BIND_EVENT_FUNC(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application() : m_Running(true), m_LayerStack()
	{
		CH_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FUNC(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		m_Camera = OrthographicCamera(1280.0f / 720.0f);

		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f
		};
		unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

		m_SquareVA.reset(VertexArray::Create());
		m_SquareVA->Bind();

		BufferLayout layout =
		{
			{ShaderDataType::Float3, "Positions"}
		};

		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(sizeof(squareVertices), squareVertices));
		squareVB->SetLayout(layout);

		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(sizeof(squareIndices), squareIndices));

		m_SquareVA->AddVertexBuffer(squareVB);
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 410 core
			
			layout(location = 0) in vec3 a_Position;

			out vec3 v_Position;

			uniform mat4 m_ViewProjection;

			void main()
			{
				v_Position = a_Position;
				gl_Position = m_ViewProjection * vec4(a_Position, 1.0f);
			}
		)";

		std::string fragmentSrc = R"(
			#version 410 core
			
			in vec3 v_Position;

			out vec4 Colour;

			void main()
			{
				Colour = vec4(v_Position * 0.5 + 0.5, 1.0f);
			}
		)";

		m_SquareShader.reset(Shader::Create(vertexSrc, fragmentSrc));
		
	}

	Application::~Application()
	{

	}

	static bool state = true;

	void Application::Run()
	{
		while (m_Running)
		{
			RendererCommand::SetClearColour({ 0.1f, 0.1f, 0.1f, 1.0f });
			RendererCommand::Clear();

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			OnUpdate();
			m_Window->OnUpdate();

		}
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowClosedEvent>(BIND_EVENT_FUNC(OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(event);
			if (event.Handled())
				break;
		}
	}

	void Application::OnUpdate()
	{
		Renderer::BeginScene(m_Camera);
		if (Input::IsKeyPressed(CH_KEY_A))
			m_Camera.SetPosition(m_Camera.GetPostion() + glm::vec3(-0.05f, 0.0f, 0.0f));
		if (Input::IsKeyPressed(CH_KEY_D))
			m_Camera.SetPosition(m_Camera.GetPostion() + glm::vec3(0.05f, 0.0f, 0.0f));
		if (Input::IsKeyPressed(CH_KEY_W))
			m_Camera.SetPosition(m_Camera.GetPostion() + glm::vec3(0.0f, 0.05f, 0.0f));
		if (Input::IsKeyPressed(CH_KEY_S))
			m_Camera.SetPosition(m_Camera.GetPostion() + glm::vec3(0.0f, -0.05f, 0.0f));
		if (Input::IsKeyPressed(CH_KEY_C))
			m_Camera.SetRotation(m_Camera.GetRotation() + -2.5f);
		if (Input::IsKeyPressed(CH_KEY_V))
			m_Camera.SetRotation(m_Camera.GetRotation() + 2.5f);

		Renderer::Submit(m_SquareVA, m_SquareShader);

		Renderer::EndScene();
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