#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Charcoal
{

	class PerspectiveCamera
	{
	public:
        PerspectiveCamera() {};
        PerspectiveCamera(float aspectRatio, float FOV, float nearPlane, float farPlane);

		void SetPosition(const glm::vec3& pos);
		void SetRotation(const glm::quat& rotation);
		void SetAspectRatio(float aspectRatio);
		void SetFOV(float FOV);
		void SetNearAndFarPlane(float nearPlane, float farPlane);

		void Rotate(float pitch, float yaw, float roll);
		void Translate(const glm::vec3& translation);

		inline const glm::vec3& GetPostion() const { return m_Pos; }
		inline float GetFOV() const { return m_FOV; }

		inline const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
		inline const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		inline const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }

	private:
		glm::mat4 m_ViewMatrix, m_ProjectionMatrix, m_ViewProjectionMatrix;
		glm::vec3 m_Pos = { 0.0f, 0.0f, 1.0f };
		glm::quat m_Rotation = {1.0f, 0.0f, 0.0f, 0.0f};
		float m_AspectRatio;
		float m_FOV;
		float m_NearPlane;
		float m_FarPlane;
	};

}
