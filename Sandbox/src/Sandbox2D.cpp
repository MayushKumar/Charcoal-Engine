#include "Sandbox2D.h"

void Sandbox2D::OnAttach()
{
	CH_PROFILE_FUNCTION();

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

	m_LogoTexture = Charcoal::Texture2D::Create("assets/textures/logo.png");
	m_BackgroundTexture = Charcoal::Texture2D::Create("assets/textures/background.png");


	Charcoal::Application::GetApplication().GetWindow().SetVSync(false);
}

void Sandbox2D::OnUpdate(Charcoal::Timestep timestep)
{
	{
		CH_PROFILE_SCOPE("Camera Update");
		m_CameraController.OnUpdate(timestep);
	}
	{
		CH_PROFILE_SCOPE("Render");
		Charcoal::RendererCommand::SetClearColour({0.1f, 0.1f, 0.1f, 1.0f});
		Charcoal::RendererCommand::ClearColourBuffer();
		
		Charcoal::Renderer2D::ResetStats();
		Charcoal::Renderer2D::BeginScene(m_CameraController.GetCamera());
		 for (int32_t y = 0; y < m_Height; y++)
		 {
		 	for (int32_t x = 0; x < m_Width; x++){
				 Charcoal::Renderer2D::DrawQuad({ (float)x * 0.27f, (float)y * 0.27f, 0.0f }, { 0.25f, 0.25f }, x % 2 == 0? m_BackgroundTexture : m_LogoTexture);
		 	}
		 }
		Charcoal::Renderer2D::EndScene();
	}
	m_Timestep = timestep;
	m_IntegralTime += timestep;
	m_Frames++;
}

void Sandbox2D::OnEvent(Charcoal::Event& event)
{
	m_CameraController.OnEvent(event);
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Renderer2D Stats");
	ImGui::Text("Max Quads: %u", Charcoal::Renderer2D::GetStats()->MaxQuads);
	ImGui::Text("Quad Count: %u", Charcoal::Renderer2D::GetStats()->QuadCount);
	ImGui::Text("Draw Calls: %u", Charcoal::Renderer2D::GetStats()->DrawCalls);
	if (m_IntegralTime >= 1.0f)
	{
		m_PrevFPS = m_Frames;
		m_IntegralTime -= 1.0f;
		m_Frames = 0;
	}
	ImGui::Text("FPS: %i", m_PrevFPS);
	ImGui::SliderInt("Width", &m_Width, 0, 500);
	ImGui::SliderInt("Height", &m_Height, 0, 500);
	ImGui::End();
}
