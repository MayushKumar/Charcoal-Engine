#pragma once

#include "RendererAPI.h"
#include "VertexArray.h"
#include "Shader.h"
#include "OrthographicCamera.h"

#include <glm/glm.hpp>

namespace Charcoal
{

	class Renderer
	{
	public:
		static void Init();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4 transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};

}