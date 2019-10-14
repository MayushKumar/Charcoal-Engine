#include "SandboxApp.h"

class ExampleLayer : public Charcoal::Layer
{

public:
	ExampleLayer() : Layer("Example Layer"){}

	void OnAttach() override
	{
		m_Camera = Charcoal::OrthographicCamera(1280.0f / 720.0f);

		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f
		};
		unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

		m_SquareVA.reset(Charcoal::VertexArray::Create());
		m_SquareVA->Bind();

		Charcoal::BufferLayout layout =
		{
			{Charcoal::ShaderDataType::Float3, "Positions"}
		};

		std::shared_ptr<Charcoal::VertexBuffer> squareVB;
		squareVB.reset(Charcoal::VertexBuffer::Create(sizeof(squareVertices), squareVertices));
		squareVB->SetLayout(layout);

		std::shared_ptr<Charcoal::IndexBuffer> squareIB;
		squareIB.reset(Charcoal::IndexBuffer::Create(sizeof(squareIndices), squareIndices));

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

		m_SquareShader.reset(Charcoal::Shader::Create(vertexSrc, fragmentSrc));
	}

	void OnUpdate(Charcoal::Timestep timestep) override
	{
		CH_TRACE("Timestep: {0}", timestep.GetSeconds());

		Charcoal::Renderer::BeginScene(m_Camera);
		if (Charcoal::Input::IsKeyPressed(CH_KEY_A))
			m_Camera.SetPosition(m_Camera.GetPostion() + glm::vec3(-2.5f * timestep, 0.0f, 0.0f));
		if (Charcoal::Input::IsKeyPressed(CH_KEY_D))
			m_Camera.SetPosition(m_Camera.GetPostion() + glm::vec3(2.5f * timestep, 0.0f, 0.0f));
		if (Charcoal::Input::IsKeyPressed(CH_KEY_W))
			m_Camera.SetPosition(m_Camera.GetPostion() + glm::vec3(0.0f, 2.5f * timestep, 0.0f));
		if (Charcoal::Input::IsKeyPressed(CH_KEY_S))
			m_Camera.SetPosition(m_Camera.GetPostion() + glm::vec3(0.0f, -2.5f * timestep, 0.0f));
		if (Charcoal::Input::IsKeyPressed(CH_KEY_C))
			m_Camera.SetRotation(m_Camera.GetRotation() + -200.0f * timestep);
		if (Charcoal::Input::IsKeyPressed(CH_KEY_V))
			m_Camera.SetRotation(m_Camera.GetRotation() + 200.0f * timestep);

		Charcoal::Renderer::Submit(m_SquareVA, m_SquareShader);

		Charcoal::Renderer::EndScene();
	}

	void OnEvent(Charcoal::Event& event) override
	{
		
	}
	
	void OnImGuiRender() override
	{

	}

private:
	Charcoal::OrthographicCamera m_Camera;
	std::shared_ptr<Charcoal::Shader> m_SquareShader;
	std::shared_ptr<Charcoal::VertexArray> m_SquareVA;

};

SandboxApp::SandboxApp()
{
	PushLayer(new ExampleLayer());
}

SandboxApp::~SandboxApp()
{

}

//Application creation

Charcoal::Application* CreateApplication()
{
	Charcoal::Application* app = new SandboxApp();
	return app;
}