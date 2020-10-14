#include "Sandbox3D.h"

void Sandbox3D::OnAttach()
{
	CH_PROFILE_FUNCTION();
	m_CameraController.SetEnabled(false);
	// m_ShaderLibrary.Add("PBR", Charcoal::ShaderManager::LoadShader("assets/shaders/SPIR-V/PBR/PBR.spr", Charcoal::ShaderLanguage::SPIRV));
	// m_ShaderLibrary.Add("PBR", Charcoal::ShaderManager::LoadShader("assets/shaders/PBR.glsl", Charcoal::ShaderLanguage::GLSL));
	Charcoal::ShaderManager::LoadShader("assets/shaders/PBR.glsl", Charcoal::ShaderLanguage::GLSL);
	// m_Model = Charcoal::CreateRef<Charcoal::Model>("assets/models/cube/cube.cmf");
	// m_Model = Charcoal::CreateRef<Charcoal::Model>("assets/models/ico_sphere/ico_sphere.cmf");
	m_Models.push_back(Charcoal::CreateRef<Charcoal::Model>("assets/models/sphere/Sphere.cmf"));
	m_Models.push_back(Charcoal::CreateRef<Charcoal::Model>("assets/models/cerberus/Cerberus.cmf"));
	// m_Models.push_back(Charcoal::CreateRef<Charcoal::Model>("assets/models/ukulele/Ukulele.cmf"));
	// m_Model = Charcoal::CreateRef<Charcoal::Model>("assets/models/monkey/monkey.cmf");
	// m_Model = Charcoal::CreateRef<Charcoal::Model>("assets/models/plane/plane.cmf");

	m_Scene = Charcoal::CreateRef<Charcoal::Scene3D>();
	
	m_Scene->SetSkyBox(Charcoal::CreateRef<Charcoal::SkyBox>("assets/textures/cubemaps/cannon/"));
	
	m_LightPos = {0.0f, 0.0f, 3.0f};
	m_LightStrength = 20.0f;
	m_Scene->m_PointLights.emplace_back(m_LightPos, glm::vec3(1.0f, 1.0f, 1.0f), m_LightStrength);
	// m_Scene->m_PointLights.emplace_back(glm::vec3(-0.3f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 10);
}

void Sandbox3D::OnUpdate(Charcoal::Timestep timestep)
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
	m_Models[m_ModelIndex]->SetTransform(glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f)));
	m_Scene->m_PointLights[0] = Charcoal::PointLight(m_LightPos, {1.0f, 1.0f, 1.0f}, m_LightStrength);
		
	{
		CH_PROFILE_SCOPE("Render");
		Charcoal::RendererCommand::SetClearColour({0.0f, 0.0f, 0.1f, 1.0f});
		Charcoal::RendererCommand::ClearColourBuffer();

		Charcoal::Renderer3D::BeginScene(m_Scene);
		Charcoal::Renderer3D::Submit(m_Models[m_ModelIndex], Charcoal::ShaderManager::GetShader("PBR"));
		Charcoal::Renderer3D::EndScene();
	}
}

void Sandbox3D::OnEvent(Charcoal::Event& event)
{
	m_CameraController.OnEvent(event);

	Charcoal::EventDispatcher dispatcher = Charcoal::EventDispatcher(event);
	dispatcher.Dispatch<Charcoal::KeyReleasedEvent>(CH_BIND_EVENT_FUNC(Sandbox3D::OnKeyReleased));
}

void Sandbox3D::OnImGuiRender()
{
	ImGui::Begin("Controls");
	ImGui::InputScalar("Model Index", ImGuiDataType_U32, &m_ModelIndex);
	ImGui::SliderFloat("Light Strength", &m_LightStrength, 0.0f, 70.0f);
	ImGui::SliderFloat3("Light Position", glm::value_ptr(m_LightPos), 0.0f, 20.0f);
	ImGui::End();
}

bool Sandbox3D::OnKeyReleased(Charcoal::KeyReleasedEvent &e)
{
	if (e.GetKeyCode() == CH_KEY_X)
	{
		m_IsCameraEnabled = !m_IsCameraEnabled;
		m_CameraController.SetEnabled(m_IsCameraEnabled);
	}
	if (e.GetKeyCode() == CH_KEY_R)
		// m_ShaderLibrary.Get("PBR")->ReadAndCompile();
		Charcoal::ShaderManager::GetShader("PBR")->ReadAndCompile();
	if (e.GetKeyCode() == CH_KEY_O)
		m_IsModelRotating = !m_IsModelRotating;
	return true;
}
