#pragma once

#include "RendererAPI.h"
#include "Charcoal/Renderer/VertexArray.h"
#include "Charcoal/Renderer/Shader.h"
#include "Charcoal/Renderer/Texture.h"
#include "Charcoal/Renderer/OrthographicCamera.h"
#include "Charcoal/Renderer/PerspectiveCamera.h"
#include "Charcoal/Renderer/Model.h"

#include <glm/glm.hpp>

namespace Charcoal
{

	class Renderer
	{
	public:
		static void Init();

		static void BeginScene(const PerspectiveCamera& camera);
		static void EndScene();

		static void Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4 transform = glm::mat4(1.0f));
		static void Submit(const Ref<Model>& model, const Ref<Shader>& shader, const glm::mat4 transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};

	class Renderer2D
	{
	public:
		static void Init();
		static void ShutDown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& scale, const glm::vec4& colour);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& scale, Ref<Texture2D> texture, const glm::vec4& tint = {1.0f, 1.0f, 1.0f, 1.0f});

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		struct Renderer2DStorage
		{
			Ref<VertexArray> QuadVertexArray;
			Ref<Shader> TextureShader;
			Ref<Texture2D> WhiteTexture;
			float ZPosCounter;
		};

		static SceneData* m_SceneData;
		static Renderer2DStorage* m_Storage;
	};

}