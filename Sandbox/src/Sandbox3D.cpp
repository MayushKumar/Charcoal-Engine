#include "chpch.h"
#include "Sandbox3D.h"

void Sandbox3D::OnAttach()
{
	CH_PROFILE_FUNCTION();

	m_ShaderLibrary = Charcoal::ShaderLibrary();

	m_Model = Charcoal::CreateRef<Charcoal::Model>("assets/models/cerberus/Cerberus.cmf");

	m_ShaderLibrary.Add("PBR", Charcoal::Shader::Create("assets/shaders/PBR.glsl"));
}

void Sandbox3D::OnUpdate(Charcoal::Timestep timestep)
{
	{
		CH_PROFILE_SCOPE("Camera Update");

		if (m_IsCameraEnabled)
			m_CameraController.OnUpdate(timestep);
	}

	if (Charcoal::Input::IsKeyPressed(CH_KEY_B))
		Charcoal::RendererCommand::SetPolygonMode(Charcoal::PolygonMode::Fill);
	if (Charcoal::Input::IsKeyPressed(CH_KEY_N))
		Charcoal::RendererCommand::SetPolygonMode(Charcoal::PolygonMode::Lines);
	if (Charcoal::Input::IsKeyPressed(CH_KEY_M))
		Charcoal::RendererCommand::SetPolygonMode(Charcoal::PolygonMode::Points);
		
	{
		CH_PROFILE_SCOPE("Render");

		Charcoal::Renderer::BeginScene(m_CameraController.GetCamera());
		Charcoal::Renderer::Submit(m_Model, m_ShaderLibrary.Get("PBR"));
		Charcoal::Renderer::EndScene();
	}
}

void Sandbox3D::OnEvent(Charcoal::Event& event)
{
	m_CameraController.OnEvent(event);

	if (Charcoal::Input::IsKeyPressed(CH_KEY_X))
	{
		Charcoal::Application::GetApplication().GetWindow().SetCursorMode(Charcoal::WindowCursorMode::Normal);
		m_IsCameraEnabled = !m_IsCameraEnabled;
	}
}

void Sandbox3D::OnImGuiRender()
{

}
