#include "chpch.h"
#include "PerspectiveCameraController.h"

#include "Charcoal/Core/Core.h"
#include "Charcoal/Input/Input.h"
#include "Charcoal/Input/KeyCodes.h"
#include "Charcoal/Core/Application.h"

namespace Charcoal
{

	PerspectiveCameraController::PerspectiveCameraController(float aspectRatio, float FOV, float nearPlane, float farPlane, bool zoomable)
		: m_Camera(aspectRatio, FOV, nearPlane, farPlane), m_IsZoomable(zoomable)
	{
		Application::GetApplication().GetWindow().SetCursorMode(WindowCursorMode::HideAndLock);
		m_LastMousePos.x = Input::GetMouseX();
		m_LastMousePos.y = Input::GetMouseY();
	}

	void PerspectiveCameraController::OnUpdate(Timestep timestep)
	{
		if (Input::IsKeyPressed(CH_KEY_A))
			m_Camera.Translate(glm::vec3(-0.03f, 0.0f, 0.0f));
		if (Input::IsKeyPressed(CH_KEY_D))
			m_Camera.Translate(glm::vec3(0.03f, 0.0f, 0.0f));
		if (Input::IsKeyPressed(CH_KEY_W))
			m_Camera.Translate(glm::vec3(0.0f, 0.0f, -0.03f));
		if (Input::IsKeyPressed(CH_KEY_S))
			m_Camera.Translate(glm::vec3(0.0f, 0.0f, 0.03f));

		glm::vec2 delta = { (Input::GetMouseX() - m_LastMousePos.x) * m_DampeningFactor, (Input::GetMouseY() - m_LastMousePos.y) * m_DampeningFactor };
		m_LastMousePos += delta;
		m_Angles += glm::vec2(-delta.y * m_MouseSenstivity, -delta.x * m_MouseSenstivity);
		m_Angles.x = m_Angles.x > 90.0f ? 90.0f : m_Angles.x;
		m_Angles.x = m_Angles.x < -90.0f ? -90.0f : m_Angles.x;
		m_Angles.y = m_Angles.y > 180.0f ? -360.0f + m_Angles.y : m_Angles.y;
		m_Angles.y = m_Angles.y < -180.0f ? 360.0f + m_Angles.y : m_Angles.y;
		m_Camera.SetRotation(glm::quat(glm::vec3(glm::radians(m_Angles.x), glm::radians(m_Angles.y), 0)));
	}

	void PerspectiveCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher = EventDispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(CH_BIND_EVENT_FUNC(PerspectiveCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(CH_BIND_EVENT_FUNC(PerspectiveCameraController::OnWindowResize));
	}

	bool PerspectiveCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		if (m_IsZoomable)
		{
			float FOV;
			FOV = m_Camera.GetFOV() + 5.0f * e.GetYOffset();
			FOV = FOV > 179.999f ? 179.999f : FOV;
			FOV = FOV < 0.0001f ? 0.0001f : FOV;
			m_Camera.SetFOV(FOV);
		}
		return false;
	}

	bool PerspectiveCameraController::OnWindowResize(WindowResizeEvent& e)
	{
		m_Camera.SetAspectRatio((float)e.GetWidth() / (float)e.GetHeight());
		return false;
	}

}