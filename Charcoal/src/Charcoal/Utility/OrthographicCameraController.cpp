#include "chpch.h"
#include "OrthographicCameraController.h"

#include "Charcoal/Core/Core.h"
#include "Charcoal/Input/Input.h"
#include "Charcoal/Input/KeyCodes.h"

namespace Charcoal
{

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation) : m_AspectRatio(aspectRatio), m_Rotation(rotation)
	{
		m_Camera = OrthographicCamera(aspectRatio);
	}

	void OrthographicCameraController::OnUpdate(Timestep timestep)
	{
		float speed = m_Camera.GetZoomLevel() * timestep;
		if (Input::IsKeyPressed(CH_KEY_A))
			m_Camera.SetPosition(m_Camera.GetPostion() + glm::vec3(-2.5f * speed, 0.0f, 0.0f));
		if (Input::IsKeyPressed(CH_KEY_D))
			m_Camera.SetPosition(m_Camera.GetPostion() + glm::vec3(2.5f * speed, 0.0f, 0.0f));
		if (Input::IsKeyPressed(CH_KEY_W))
			m_Camera.SetPosition(m_Camera.GetPostion() + glm::vec3(0.0f, 2.5f * speed, 0.0f));
		if (Input::IsKeyPressed(CH_KEY_S))
			m_Camera.SetPosition(m_Camera.GetPostion() + glm::vec3(0.0f, -2.5f * speed, 0.0f));

		if (m_Rotation)
		{
			if (Input::IsKeyPressed(CH_KEY_C))
				m_Camera.SetRotation(m_Camera.GetRotation() + -200.0f * timestep);
			if (Input::IsKeyPressed(CH_KEY_V))
				m_Camera.SetRotation(m_Camera.GetRotation() + 200.0f * timestep);
		}
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(CH_BIND_EVENT_FUNC(OrthographicCameraController::OnKeyPressed));
		dispatcher.Dispatch<MouseScrolledEvent>(CH_BIND_EVENT_FUNC(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(CH_BIND_EVENT_FUNC(OrthographicCameraController::OnWindowResize));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.1f;
		m_Camera.SetZoomLevel(m_ZoomLevel);
		return true;
	}

	bool OrthographicCameraController::OnKeyPressed(KeyPressedEvent& e)
	{
		return false;
	}

	bool OrthographicCameraController::OnWindowResize(WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetAspectRatio(m_AspectRatio);
		return false;
	}

}