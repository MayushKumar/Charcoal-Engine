#include "EditorLayer.h"

void EditorLayer::OnAttach()
{
	CH_PROFILE_FUNCTION();

	m_Scene = Charcoal::CreateRef<Charcoal::Scene3D>();
	
    Charcoal::ShaderManager::LoadShader("../../Sandbox/assets/shaders/PBR.glsl", Charcoal::ShaderLanguage::GLSL);
	// m_Model = Charcoal::CreateRef<Charcoal::Model>("assets/models/cube/cube.cmf");
	// m_Model = Charcoal::CreateRef<Charcoal::Model>("assets/models/ico_sphere/ico_sphere.cmf");
	// m_Model = Charcoal::CreateRef<Charcoal::Model>("assets/models/ukulele/Ukulele.cmf");
	m_Model = Charcoal::CreateRef<Charcoal::Model>("../../Sandbox/assets/models/cerberus/Cerberus.cmf");
	// m_Model = Charcoal::CreateRef<Charcoal::Model>("../../Sandbox/assets/models/sphere/Sphere.cmf");
	// m_Model = Charcoal::CreateRef<Charcoal::Model>("assets/models/monkey/monkey.cmf");
	// m_Model = Charcoal::CreateRef<Charcoal::Model>("assets/models/plane/plane.cmf");
	
	m_LightPos = {0.0f, 0.0f, 3.0f};
	m_LightStrength = 20.0f;
	m_LightColour = {1.0f, 1.0f, 1.0f};
	m_Scene->m_PointLights.emplace_back(m_LightPos, m_LightColour, m_LightStrength);
	m_EditorViewport = Charcoal::Framebuffer::Create({1280, 720});
	
	m_CameraController.SetEnabled(false);
	m_IsCameraEnabled = false;

	Charcoal::Application::GetApplication().GetWindow().SetVSync(false);
}

void EditorLayer::OnUpdate(Charcoal::Timestep timestep)
{
	{
		CH_PROFILE_SCOPE("Camera Update");

		if (m_IsCameraEnabled)
			m_CameraController.OnUpdate(timestep);
	}
	m_Scene->m_Camera = m_CameraController.GetCamera();
	
	if (Charcoal::Input::IsKeyPressed(CH_KEY_B))
		Charcoal::RendererCommand::SetPolygonMode(Charcoal::PolygonMode::Fill);
	if (Charcoal::Input::IsKeyPressed(CH_KEY_N))
		Charcoal::RendererCommand::SetPolygonMode(Charcoal::PolygonMode::Lines);
	if (Charcoal::Input::IsKeyPressed(CH_KEY_M))
		Charcoal::RendererCommand::SetPolygonMode(Charcoal::PolygonMode::Points);
	
	static float angle = 0;
	if (m_IsModelRotating)
		angle += timestep * glm::radians(45.0f);
	m_Model->SetTransform(glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f)));

	m_Scene->m_PointLights[0] = Charcoal::PointLight(m_LightPos, m_LightColour, m_LightStrength);
		
	{
		CH_PROFILE_SCOPE("Render");
		Charcoal::RendererCommand::SetClearColour({0.0f, 0.0f, 0.1f, 1.0f});
		Charcoal::RendererCommand::ClearColourBuffer();
		
		m_EditorViewport->Bind();
		Charcoal::RendererCommand::SetClearColour({0.0f, 0.0f, 0.1f, 1.0f});
		Charcoal::RendererCommand::ClearColourBuffer();
		Charcoal::RendererCommand::ClearDepthBuffer();

		Charcoal::Renderer3D::BeginScene(m_Scene);
		Charcoal::Renderer3D::Submit(m_Model, Charcoal::ShaderManager::GetShader("PBR"));
		Charcoal::Renderer3D::EndScene();
		m_EditorViewport->Unbind();
	}
}

void EditorLayer::OnEvent(Charcoal::Event& event)
{
	m_CameraController.OnEvent(event);

	Charcoal::EventDispatcher dispatcher = Charcoal::EventDispatcher(event);
	dispatcher.Dispatch<Charcoal::KeyReleasedEvent>(CH_BIND_EVENT_FUNC(EditorLayer::OnKeyReleased));
}

void EditorLayer::OnImGuiRender()
{
	ImGui::ShowDemoWindow();
	
	ImGui::Begin("Light Control");
	ImGui::SliderFloat("Light Strength", &m_LightStrength, 0.0f, 70.0f);
	ImGui::SliderFloat3("Light Position", glm::value_ptr(m_LightPos), 0.0f, 20.0f);
	ImGui::SliderFloat3("Light Colour", glm::value_ptr(m_LightColour), 0.0f, 1.0f);
	ImGui::End();
	
	ImGui::Begin("Viewport");
	ImGui::Image((void*)m_EditorViewport->GetColourAttachment(0), {1280, 720}, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
	if(ImGui::IsWindowFocused())
		ImGui::CaptureMouseFromApp();
	ImGui::End();
}

bool EditorLayer::OnKeyReleased(Charcoal::KeyReleasedEvent &e)
{
	if (e.GetKeyCode() == CH_KEY_X)
	{
		m_IsCameraEnabled = !m_IsCameraEnabled;
		m_CameraController.SetEnabled(m_IsCameraEnabled);
	}
	if (e.GetKeyCode() == CH_KEY_R)
		Charcoal::ShaderManager::GetShader("PBR")->ReadAndCompile();
	if (e.GetKeyCode() == CH_KEY_O)
		m_IsModelRotating = !m_IsModelRotating;
	return true;
}
