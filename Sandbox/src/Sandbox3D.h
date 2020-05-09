#pragma once

#include <Charcoal.h>

class Sandbox3D : public Charcoal::Layer
{

public:
	Sandbox3D() : Layer("Example Layer"), m_CameraController(1280.0f / 720.0f, 45.0f, 0.01f, 1000.0f, true)
	{
	}

	void OnAttach() override;
	void OnUpdate(Charcoal::Timestep timestep) override;
	void OnEvent(Charcoal::Event& event) override;
	void OnImGuiRender() override;

private:
	Charcoal::Ref<Charcoal::Model> m_Model;
	Charcoal::PerspectiveCameraController m_CameraController;

	Charcoal::ShaderLibrary m_ShaderLibrary;

	bool m_IsCameraEnabled  = true;
};