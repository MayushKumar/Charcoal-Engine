#include "chpch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Charcoal
{

	OrthographicCamera::OrthographicCamera(float aspectRatio) : m_AspectRatio(aspectRatio)
	{
		CH_PROFILE_FUNCTION();

		m_ViewMatrix = glm::translate(glm::mat4(1.0f), -m_Pos);
		m_ProjectionMatrix = glm::ortho(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel, 1.0f, -1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetPosition(const glm::vec3& pos)
	{
		CH_PROFILE_FUNCTION();

		m_Pos = pos;
		m_ViewMatrix = glm::rotate(glm::mat4(1.0f), -m_Rotation, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::translate(glm::mat4(1.0f), -m_Pos);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetRotation(float rotation)
	{
		CH_PROFILE_FUNCTION();

		m_Rotation = glm::radians(rotation);
		m_ViewMatrix = glm::rotate(glm::mat4(1.0f), -m_Rotation, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::translate(glm::mat4(1.0f), -m_Pos);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetAspectRatio(float aspectRatio)
	{
		CH_PROFILE_FUNCTION();

		m_AspectRatio = aspectRatio;
		m_ProjectionMatrix = glm::ortho(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel, 1.0f, -1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetZoomLevel(float zoomLevel)
	{
		CH_PROFILE_FUNCTION();

		m_ZoomLevel = zoomLevel;
		m_ProjectionMatrix = glm::ortho(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel, 1.0f, -1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}
