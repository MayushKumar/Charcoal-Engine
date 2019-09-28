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
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const std::weak_ptr<VertexArray>& vertexArray, const std::weak_ptr<Shader>& shader);

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};

}