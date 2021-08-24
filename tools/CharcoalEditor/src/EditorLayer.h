#pragma once

#include <Charcoal.h>
#include "Utils/ViewportCameraController.h"
#include "glm/fwd.hpp"

class EditorLayer : public Charcoal::Layer
{

public:
	EditorLayer() : Layer("Example Layer"), m_CameraController(m_EditorViewportSize.x / m_EditorViewportSize.y, 45.0f, 0.01f, 100.0f, true)
	{
	}

	void OnAttach() override;
	void OnUpdate(Charcoal::Timestep timestep) override;
	void OnEvent(Charcoal::Event &event) override;
	void OnImGuiRender() override;

	bool OnKeyReleased(Charcoal::KeyReleasedEvent& e);

private:
	void UpdateImGuiDockspace();

private:

	Charcoal::Ref<Charcoal::Framebuffer> m_EditorViewport;
	glm::vec2 m_EditorViewportSize = {1280.f, 720.0f};

	Charcoal::Ref<Charcoal::Scene3D> m_Scene;

	Charcoal::ViewportCameraController m_CameraController;
	Charcoal::Ref<Charcoal::Model> m_Model;
	
	glm::vec3 m_LightPos;
	glm::vec3 m_LightColour;

	float m_LightStrength;
	
	bool m_IsCameraEnabled = true;
	bool m_IsModelRotating = true;

};
