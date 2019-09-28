#pragma once

#include <glm/glm.hpp>

namespace Charcoal
{

	class OrthographicCamera
	{
	public:
		OrthographicCamera() {}
		OrthographicCamera(float aspectRatio);

		void SetPosition(const glm::vec3& pos);
		void SetRotation(float rotation);
		void SetZoomLevel(float zoomLevel);

		inline const glm::vec3& GetPostion() { return m_Pos; }
		inline float GetRotation() { return glm::degrees(m_Rotation); }

		inline const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	private:
		glm::mat4 m_ViewMatrix, m_ProjectionMatrix, m_ViewProjectionMatrix;
		glm::vec3 m_Pos = { 0.0f, 0.0f, 1.0f };
		float m_Rotation = 0.0f;
		float m_AspectRatio = 0.0f;
		float m_ZoomLevel = 1.0f;
	};

}