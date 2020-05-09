#pragma once

#include "Charcoal/Renderer/OrthographicCamera.h"
#include "Charcoal/Core/Timestep.h"
#include "Charcoal/Events/Event.h"
#include "Charcoal/Events/MouseEvent.h"
#include "Charcoal/Events/KeyEvent.h"
#include "Charcoal/Events/ApplicationEvent.h"

namespace Charcoal
{

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep timestep);
		void OnEvent(Event& e);
		inline OrthographicCamera& GetCamera() { return m_Camera; }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		bool m_Rotation;
		OrthographicCamera m_Camera;
	};

}