#include "SandboxApp.h"

class ExampleLayer : public Charcoal::Layer
{

public:
	ExampleLayer() : Layer("Example Layer"), m_CameraController(1280.0f / 720.0f, true)
	{
	}

	void OnAttach() override
	{
		m_ShaderLibrary = Charcoal::ShaderLibrary();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
		};
		unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

		m_SquareVA.reset(Charcoal::VertexArray::Create());
		m_SquareVA->Bind();

		Charcoal::BufferLayout layout =
		{
			{Charcoal::ShaderDataType::Float3, "a_Position"},
			{Charcoal::ShaderDataType::Float2, "a_TexCoord"}
		};

		squareVB.reset(Charcoal::VertexBuffer::Create(sizeof(squareVertices), squareVertices));
		squareVB->SetLayout(layout);

		squareIB.reset(Charcoal::IndexBuffer::Create(sizeof(squareIndices), squareIndices));

		m_SquareVA->AddVertexBuffer(squareVB);
		m_SquareVA->SetIndexBuffer(squareIB);

		m_ShaderLibrary.Load("assets/shaders/Texture.glsl");
		m_TextureShader = m_ShaderLibrary.Get("Texture");
		m_TextureShader->SetInt("u_Texture", 0);
		m_LogoTexture = Charcoal::Texture2D::Create("assets/textures/logo.png");
		m_BackgroundTexture = Charcoal::Texture2D::Create("assets/textures/background.png");
	}

	void OnUpdate(Charcoal::Timestep timestep) override
	{
		m_CameraController.OnUpdate(timestep);

		Charcoal::Renderer::BeginScene(m_CameraController.GetCamera());

		m_BackgroundTexture->Bind();
		Charcoal::Renderer::Submit(m_SquareVA, m_TextureShader);
		m_LogoTexture->Bind();
		Charcoal::Renderer::Submit(m_SquareVA, m_TextureShader);

		Charcoal::Renderer::EndScene();
	}

	void OnEvent(Charcoal::Event& event) override
	{
		m_CameraController.OnEvent(event);
	}

	void OnImGuiRender() override
	{
		
	}

private:
	Charcoal::OrthographicCameraController m_CameraController;
	Charcoal::Ref<Charcoal::VertexBuffer> squareVB;
	Charcoal::Ref<Charcoal::IndexBuffer> squareIB;
	Charcoal::Ref<Charcoal::Shader> m_TextureShader;
	Charcoal::Ref<Charcoal::Shader> m_FlatColourShader;
	Charcoal::Ref<Charcoal::VertexArray> m_SquareVA;
	Charcoal::Ref<Charcoal::Texture> m_BackgroundTexture;
	Charcoal::Ref<Charcoal::Texture> m_LogoTexture;

	Charcoal::ShaderLibrary m_ShaderLibrary;

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