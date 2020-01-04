#include "Sandbox2D.h"

void Sandbox2D::OnAttach()
{
	CH_PROFILE_FUNCTION();

	m_ShaderLibrary = Charcoal::ShaderLibrary();

	float squareVertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
	};
	unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

	m_SquareVA = Charcoal::VertexArray::Create();
	m_SquareVA->Bind();

	Charcoal::BufferLayout layout =
	{
		{Charcoal::ShaderDataType::Float3, "a_Position"},
		{Charcoal::ShaderDataType::Float2, "a_TexCoord"}
	};

	squareVB = Charcoal::VertexBuffer::Create(sizeof(squareVertices), squareVertices);
	squareVB->SetLayout(layout);

	squareIB = Charcoal::IndexBuffer::Create(sizeof(squareIndices), squareIndices);

	m_SquareVA->AddVertexBuffer(squareVB);
	m_SquareVA->SetIndexBuffer(squareIB);

	m_ShaderLibrary.Load("assets/shaders/Texture.glsl");
	m_TextureShader = m_ShaderLibrary.Get("Texture");
	m_TextureShader->SetInt("u_Texture", 0);
	m_LogoTexture = Charcoal::Texture2D::Create("assets/textures/logo.png");
	m_BackgroundTexture = Charcoal::Texture2D::Create("assets/textures/background.png");
}

void Sandbox2D::OnUpdate(Charcoal::Timestep timestep)
{
	{
		CH_PROFILE_SCOPE("Camera Update");
		m_CameraController.OnUpdate(timestep);
	}
	{
		CH_PROFILE_SCOPE("Render");
		Charcoal::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Charcoal::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 0.5f, 0.5f }, { 0.2f, 0.8f, 0.9f, 1.0f });
		Charcoal::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 0.5f, 0.5f }, m_BackgroundTexture);
		Charcoal::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 0.5f, 0.5f }, m_LogoTexture);
		Charcoal::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnEvent(Charcoal::Event& event)
{
	m_CameraController.OnEvent(event);
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Profile Results");
	for (Charcoal::ProfileResult& result : m_ProfileResults)
	{
		char label[50];
		strcpy_s(label, "%.3fms ");
		strcat_s(label, result.Name);
		ImGui::Text(label, result.End - result.Start);
	}
	m_ProfileResults.clear();
	ImGui::End();
}
