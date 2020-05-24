#pragma once

#include <Charcoal.h>

class Sandbox2D : public Charcoal::Layer
{

public:
	Sandbox2D() : Layer("Example Layer"), m_CameraController(1280.0f / 720.0f, true)
	{
	}

	void OnAttach() override;
	void OnUpdate(Charcoal::Timestep timestep) override;
	void OnEvent(Charcoal::Event& event) override;
	void OnImGuiRender() override;

private:
	int32_t m_Width = 0, m_Height = 0;
	float m_IntegralTime = 0.0f;
	float m_Timestep = 0.0f;
	int32_t m_Frames = 0;
	int32_t m_PrevFPS;

	Charcoal::OrthographicCameraController m_CameraController;
	Charcoal::Ref<Charcoal::VertexBuffer> squareVB;
	Charcoal::Ref<Charcoal::IndexBuffer> squareIB;
	Charcoal::Ref<Charcoal::VertexArray> m_SquareVA;
	Charcoal::Ref<Charcoal::Texture2D> m_BackgroundTexture;
	Charcoal::Ref<Charcoal::Texture2D> m_LogoTexture;

	Charcoal::ShaderLibrary m_ShaderLibrary;

};