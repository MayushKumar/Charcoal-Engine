#include "Charcoal/Events/MouseEvent.h"
#include "chpch.h"
#include "ViewportCameraController.h"

#include "Charcoal/Core/Core.h"
#include "Charcoal/Input/Input.h"
#include "Charcoal/Input/KeyCodes.h"
#include "Charcoal/Input/MouseCodes.h"
#include "Charcoal/Core/Application.h"

namespace Charcoal
{

	ViewportCameraController::ViewportCameraController(float aspectRatio, float FOV, float nearPlane, float farPlane, bool zoomable)
		: m_Camera(aspectRatio, FOV, nearPlane, farPlane), m_IsZoomable(zoomable), m_Enabled(true)
	{
		// Application::GetApplication().GetWindow().SetCursorMode(WindowCursorMode::HideAndLock);
		m_LastMousePos.x = Input::GetMouseX();
		m_LastMousePos.y = Input::GetMouseY();

		m_Camera.SetPosition({0.0f, 0.0f, 5.0f});
	}

	void ViewportCameraController::OnUpdate(Timestep timestep)
	{

	}

	void ViewportCameraController::OnEvent(Event &e)
	{
		EventDispatcher dispatcher = EventDispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(CH_BIND_EVENT_FUNC(ViewportCameraController::OnMouseScrolled));
		dispatcher.Dispatch<MouseButtonPressedEvent>(CH_BIND_EVENT_FUNC(ViewportCameraController::OnMouseButtonPressed));
		dispatcher.Dispatch<MouseMovedEvent>(CH_BIND_EVENT_FUNC(ViewportCameraController::OnMouseMoved));
		dispatcher.Dispatch<WindowResizeEvent>(CH_BIND_EVENT_FUNC(ViewportCameraController::OnWindowResize));
	}

	void ViewportCameraController::SetEnabled(bool enabled)
	{
		m_Enabled = enabled;
	}

	void ViewportCameraController::SetAspectRatio(float aspectRatio)
	{
		m_Camera.SetAspectRatio(aspectRatio);
	}

	bool ViewportCameraController::OnMouseScrolled(MouseScrolledEvent &e)
	{
		// if (m_IsZoomable && m_Enabled)
		// {
		// 	float FOV;
		// 	FOV = m_Camera.GetFOV() + 5.0f * e.GetYOffset();
		// 	FOV = FOV > 179.999f ? 179.999f : FOV;
		// 	FOV = FOV < 0.0001f ? 0.0001f : FOV;
		// 	m_Camera.SetFOV(FOV);
		// 	return true;
		// }
		m_Camera.Translate(glm::vec3(0.0f, 0.0f, e.GetYOffset() * -0.2f));
		return false;
	}

	bool ViewportCameraController::OnMouseButtonPressed(MouseButtonPressedEvent &e)
	{
		// if (m_Enabled)
		// {
		// 	switch (e.GetMouseButtonCode())
		// 	{
		// 	case CH_MOUSE_BUTTON_1:
		// 		if (Input::IsKeyPressed(CH_KEY_LEFT_SHIFT))
		// 		{
		// 			glm::vec2 delta = {Input::GetMouseX() - m_LastMousePos.x, Input::GetMouseY() - m_LastMousePos.y};
		// 			m_Camera.Translate(glm::vec3(delta * 0.0001f, 0.0f));
		// 		}

		// 		// glm::vec2 delta = {(Input::GetMouseX() - m_LastMousePos.x) * m_DampeningFactor, (Input::GetMouseY() - m_LastMousePos.y) * m_DampeningFactor};
		// 		// m_LastMousePos += delta;
		// 		// m_Angles += glm::vec2(-delta.y * m_MouseSenstivity, -delta.x * m_MouseSenstivity);
		// 		// m_Angles.x = m_Angles.x > 90.0f ? 90.0f : m_Angles.x;
		// 		// m_Angles.x = m_Angles.x < -90.0f ? -90.0f : m_Angles.x;
		// 		// m_Angles.y = m_Angles.y > 180.0f ? -360.0f + m_Angles.y : m_Angles.y;
		// 		// m_Angles.y = m_Angles.y < -180.0f ? 360.0f + m_Angles.y : m_Angles.y;
		// 		// m_Camera.SetRotation(glm::quat(glm::vec3(glm::radians(m_Angles.x), glm::radians(m_Angles.y), 0)));
		// 	}
		// }
		
		return false;
	}

	bool ViewportCameraController::OnMouseMoved(MouseMovedEvent &e)
	{
		if (m_Enabled && Input::IsKeyPressed(CH_KEY_LEFT_SHIFT))
		{
			if(Input::IsMouseButtonPressed(CH_MOUSE_BUTTON_1))
			{
				glm::vec2 delta = {Input::GetMouseX() - m_LastMousePos.x, -(Input::GetMouseY() - m_LastMousePos.y)};
				m_Camera.Translate(glm::vec3(delta * 0.001f, 0.0f));
			}
		}
		m_LastMousePos = {e.GetX(), e.GetY()};
		return true;
	}

	bool ViewportCameraController::OnWindowResize(WindowResizeEvent &e)
	{
		m_Camera.SetAspectRatio((float)e.GetWidth() / (float)e.GetHeight());
		return false;
	}

	
}
