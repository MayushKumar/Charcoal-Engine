#pragma once

#include <Charcoal.h>

class Sandbox3D : public Charcoal::Layer
{

public:
	Sandbox3D() : Layer("Example Layer"), m_CameraController(1280.0f / 720.0f, 70.0f, 0.1f, 100.0f, true)
	{
	}

	void OnAttach() override;
	void OnUpdate(Charcoal::Timestep timestep) override;
	void OnEvent(Charcoal::Event &event) override;
	void OnImGuiRender() override;

	bool OnKeyReleased(Charcoal::KeyReleasedEvent& e);

private:
	Charcoal::Ref<Charcoal::Scene3D> m_Scene;

	std::vector<Charcoal::Ref<Charcoal::Model>> m_Models;
	uint32_t m_ModelIndex = 0;
	Charcoal::PerspectiveCameraController m_CameraController;
	glm::vec3 m_LightPos;
	float m_LightStrength;

	bool m_IsCameraEnabled = true;
	bool m_IsModelRotating = true;
};
