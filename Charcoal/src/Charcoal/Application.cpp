#include <chpch.h>

#include "Core.h"
#include "Application.h"

#include <glad/glad.h>

namespace Charcoal {

#define BIND_EVENT_FUNC(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case Charcoal::ShaderDataType::None:	return GL_NONE;
			case Charcoal::ShaderDataType::Float:	return GL_FLOAT;
			case Charcoal::ShaderDataType::Float2:	return GL_FLOAT;
			case Charcoal::ShaderDataType::Float3:	return GL_FLOAT;
			case Charcoal::ShaderDataType::Float4:	return GL_FLOAT;
			case Charcoal::ShaderDataType::Mat3:	return GL_FLOAT;
			case Charcoal::ShaderDataType::Mat4:	return GL_FLOAT;
			case Charcoal::ShaderDataType::Int:		return GL_INT;
			case Charcoal::ShaderDataType::Int2:	return GL_INT;
			case Charcoal::ShaderDataType::Int3:	return GL_INT;
			case Charcoal::ShaderDataType::Int4:	return GL_INT;
			case Charcoal::ShaderDataType::Bool:	return GL_BOOL;
		}
		CH_CORE_ASSERT(false, "Unkown ShaderDataType!");
		return GL_NONE;
	}

	Application::Application() : m_Running(true), m_LayerStack()
	{
		CH_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FUNC(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		float vertices[7 * 3] = {
			 0.0f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f
		};

		m_VertexBuffer.reset(VertexBuffer::Create(sizeof(vertices), vertices));
		m_VertexBuffer->Bind();
		//m_VertexBuffer->SetLayout(layout);

		BufferLayout bufferLayout = {
			{ ShaderDataType::Float3, "a_Postion" },
			{ ShaderDataType::Float4, "a_Colour" }
		};
		m_VertexBuffer->SetLayout(bufferLayout);

		uint32_t index = 0;
		const auto& layout = m_VertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glVertexAttribPointer(index, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.Type), element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)element.Offset);
			glEnableVertexAttribArray(index);
			index++;
		}

		unsigned int indices[3] = { 0, 1, 2 };
		m_IndexBuffer.reset(IndexBuffer::Create(sizeof(indices), indices));
		m_IndexBuffer->Bind();

		std::string vertexSrc = R"(
			#version 410 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Colour;			

			out vec3 v_Position;
			out vec4 v_Colour;	

			void main()
			{
				v_Position = a_Position;
				v_Colour = a_Colour;
				gl_Position = vec4(a_Position, 1.0f);
			}
		)";

		std::string fragmentSrc = R"(
			#version 410 core
			
			in vec3 v_Position;
			in vec4 v_Colour;

			out vec4 Colour;

			void main()
			{
				Colour = v_Colour;
			}
		)";

		m_Shader.reset(Shader::Create(vertexSrc, fragmentSrc));
		
	}

	Application::~Application()
	{

	}

	static bool state = true;

	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

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
		m_Shader->Bind();
		glBindVertexArray(m_VertexArray);
		glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
		m_Shader->Unbind();
	}

	bool Application::OnWindowClose(WindowClosedEvent& event)
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