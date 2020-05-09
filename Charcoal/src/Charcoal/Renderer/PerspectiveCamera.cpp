#include "chpch.h"
#include "PerspectiveCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Charcoal
{

	PerspectiveCamera::PerspectiveCamera(float aspectRatio, float FOV, float nearPlane, float farPlane) 
		: m_AspectRatio(aspectRatio), m_FOV(FOV), m_NearPlane(nearPlane), m_FarPlane(farPlane)
	{
		m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearPlane, m_FarPlane);
		m_ViewMatrix = glm::translate(glm::mat4(1.0f), -m_Pos);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera::SetPosition(const glm::vec3& pos)
	{
		m_Pos = pos;
		m_ViewMatrix = glm::mat4_cast(glm::inverse(m_Rotation)) * glm::translate(glm::mat4(1.0f), -m_Pos);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera::SetRotation(const glm::quat& rotation)
	{
		m_Rotation = rotation;
		m_ViewMatrix = glm::mat4_cast(glm::inverse(m_Rotation)) * glm::translate(glm::mat4(1.0f), -m_Pos);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera::SetAspectRatio(float aspectRatio)
	{
		m_AspectRatio = aspectRatio;
		m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearPlane, m_FarPlane);
	}

	void PerspectiveCamera::SetFOV(float FOV)
	{
		m_FOV = FOV;
		m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearPlane, m_FarPlane);
	}

	void PerspectiveCamera::SetNearAndFarPlane(float nearPlane, float farPlane)
	{
		m_NearPlane = nearPlane;
		m_FarPlane = farPlane;
		m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearPlane, m_FarPlane);
	}

	void PerspectiveCamera::Rotate(float pitch, float yaw, float roll)
	{
		SetRotation(m_Rotation * glm::quat({ glm::radians(pitch), glm::radians(yaw), glm::radians(roll) }));
	}

	void PerspectiveCamera::Translate(const glm::vec3& translation)
	{
		SetPosition(m_Pos + m_Rotation * translation);
	}

}