#pragma once

#include <Charcoal/Renderer/PerspectiveCamera.h>

#include "Charcoal/Core/Timestep.h"
#include "Charcoal/Events/Event.h"
#include "Charcoal/Events/MouseEvent.h"
#include "Charcoal/Events/KeyEvent.h"
#include "Charcoal/Events/ApplicationEvent.h"

#include <glm/glm.hpp>

namespace Charcoal
{

	class PerspectiveCameraController
	{
	public:
		PerspectiveCameraController(float aspectRatio, float FOV, float nearPlane, float farPlane, bool zoomable = false);

		void OnUpdate(Timestep timestep);
		void OnEvent(Event& e);

		void SetEnabled(bool enabled);

		inline PerspectiveCamera& GetCamera() { return m_Camera; }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		PerspectiveCamera m_Camera;
		bool m_IsZoomable;
		bool m_Enabled;
		glm::vec2 m_LastMousePos;
		glm::vec2 m_Angles = {0.0f, 0.0f};
		float m_DampeningFactor = 0.3f;
		float m_MouseSenstivity = 0.2f;
	};

}