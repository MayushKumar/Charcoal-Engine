#include "EditorLayer.h"
#include "Charcoal/Core/Application.h"
#include "Charcoal/Core/Core.h"
#include "Charcoal/Core/Log.h"
#include "Charcoal/Renderer/Model.h"
#include "Charcoal/Renderer/Renderer3D.h"
#include "Charcoal/Renderer/RendererCommand.h"

#include <imgui.h>

void EditorLayer::OnAttach()
{
	CH_PROFILE_FUNCTION();
	m_Scene = Charcoal::CreateRef<Charcoal::Scene3D>();
	
    Charcoal::ShaderManager::LoadShader("../../Sandbox/assets/shaders/PBR.glsl", Charcoal::ShaderLanguage::GLSL);
	// m_Model = Charcoal::CreateRef<Charcoal::Model>("assets/models/cube/cube.cmf");
	// m_Model = Charcoal::CreateRef<Charcoal::Model>("assets/models/ico_sphere/ico_sphere.cmf");
	// m_Model = Charcoal::CreateRef<Charcoal::Model>("assets/models/ukulele/Ukulele.cmf");
	m_Model = Charcoal::ModelManager::LoadModel("../../Sandbox/assets/models/cerberus/Cerberus.cmf");
	// m_Model = Charcoal::CreateRef<Charcoal::Model>("../../Sandbox/assets/models/sphere/Sphere.cmf");
	// m_Model = Charcoal::CreateRef<Charcoal::Model>("assets/models/monkey/monkey.cmf");
	// m_Model = Charcoal::CreateRef<Charcoal::Model>("assets/models/plane/plane.cmf");
	
	m_LightPos = {0.0f, 0.0f, 3.0f};
	m_LightStrength = 20.0f;
	m_LightColour = {1.0f, 1.0f, 1.0f};
	m_Scene->m_PointLights.emplace_back(m_LightPos, m_LightColour, m_LightStrength);
	m_Scene->SetSkyBox(Charcoal::CreateRef<Charcoal::SkyBox>("assets/cubemaps/cannon/"));
	m_EditorViewportSize = {1280.0f, 720.0f};
	m_EditorViewport = Charcoal::Framebuffer::Create({(uint32_t)m_EditorViewportSize.x, (uint32_t)m_EditorViewportSize.y,
													  {{Charcoal::FramebufferTextureFormat::RGBA16F}}});
	
	m_CameraController.SetEnabled(true);
	m_IsCameraEnabled = true;
}

void EditorLayer::OnUpdate(Charcoal::Timestep timestep)
{
	Charcoal::RendererCommand::SetViewport(0, 0, m_EditorViewportSize.x, m_EditorViewportSize.y);
	m_EditorViewport->Resize(m_EditorViewportSize.x, m_EditorViewportSize.y);
	m_CameraController.SetAspectRatio(m_EditorViewportSize.x / m_EditorViewportSize.y);
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
		
		Charcoal::FramebufferStack::Push(m_EditorViewport);

		Charcoal::RendererCommand::SetClearColour({0.0f, 0.0f, 0.1f, 1.0f});
		Charcoal::RendererCommand::ClearColourBuffer();
		Charcoal::RendererCommand::ClearDepthBuffer();

		Charcoal::Renderer3D::BeginScene(m_Scene);
		Charcoal::Renderer3D::Submit(m_Model, Charcoal::ShaderManager::GetShader("PBR"));
		Charcoal::Renderer3D::EndScene();

		Charcoal::FramebufferStack::Pop();
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
	UpdateImGuiDockspace();
	ImGui::ShowDemoWindow();
	
	ImGuiIO& io = ImGui::GetIO();
	ImGui::Begin("Light Control");
	ImGui::SliderFloat("Light Strength", &m_LightStrength, 0.0f, 70.0f);
	ImGui::SliderFloat3("Light Position", glm::value_ptr(m_LightPos), 0.0f, 20.0f);
	ImGui::SliderFloat3("Light Colour", glm::value_ptr(m_LightColour), 0.0f, 1.0f);
	ImGui::End();
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

	ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollbar);
	Charcoal::Application::GetApplication().SetImGuiBlockEvents(!ImGui::IsWindowFocused());
	io.WantCaptureMouse = ImGui::IsWindowFocused();
	m_EditorViewportSize = {ImGui::GetWindowWidth(), ImGui::GetWindowHeight()};
	ImGui::Image((void*)m_EditorViewport->GetColourAttachmentID(0), {m_EditorViewportSize.x, m_EditorViewportSize.y},
				 ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
	ImGui::End();
	ImGui::PopStyleVar();
}

bool EditorLayer::OnKeyReleased(Charcoal::KeyReleasedEvent &e)
{
	if (e.GetKeyCode() == CH_KEY_R)
		Charcoal::ShaderManager::GetShader("PBR")->ReadAndCompile();
	if (e.GetKeyCode() == CH_KEY_O)
		m_IsModelRotating = !m_IsModelRotating;
	return true;
}

void EditorLayer::UpdateImGuiDockspace()
{
	ImGuiDockNodeFlags dockspaceFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoBackground;
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowSize(viewport->GetWorkSize());
	ImGui::SetNextWindowPos(viewport->GetWorkPos());

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::Begin("Dockspace", nullptr, dockspaceFlags);
	ImGui::DockSpace(ImGui::GetID("Dockspace"), ImVec2(0.0f, 0.0f), 0);
	ImGui::End();
	ImGui::PopStyleVar();
}
