#pragma once

#include <Charcoal.h>

class EditorLayer : public Charcoal::Layer
{

public:
	EditorLayer() : Layer("Example Layer"), m_CameraController(1280.0f / 720.0f, 45.0f, 0.01f, 1000.0f, true)
	{
	}

	void OnAttach() override;
	void OnUpdate(Charcoal::Timestep timestep) override;
	void OnEvent(Charcoal::Event &event) override;
	void OnImGuiRender() override;

	bool OnKeyReleased(Charcoal::KeyReleasedEvent& e);

private:

	Charcoal::Ref<Charcoal::Framebuffer> m_EditorViewport;
	
	Charcoal::Ref<Charcoal::Scene3D> m_Scene;

	Charcoal::Ref<Charcoal::Model> m_Model;
	Charcoal::PerspectiveCameraController m_CameraController;
	glm::vec3 m_LightPos;
	glm::vec3 m_LightColour;

	float m_LightStrength;
	
	bool m_IsCameraEnabled = true;
	bool m_IsModelRotating = true;
};
