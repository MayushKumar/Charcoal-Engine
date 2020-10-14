#pragma once

#include "Charcoal/Renderer/RendererAPI.h"
#include "Charcoal/Renderer/RendererCommand.h"
#include "Charcoal/Renderer/VertexArray.h"
#include "Charcoal/Renderer/ShaderManager.h"
#include "Charcoal/Renderer/Texture.h"
#include "Charcoal/Utility/OrthographicCameraController.h"

#include <glm/glm.hpp>

namespace Charcoal
{
    class Renderer2D
	{
	public:
		struct Statistics
		{
			uint32_t MaxQuads;
			uint32_t QuadCount;
			uint32_t DrawCalls;
		};

	public:
		static void Init();
		static void ShutDown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void DrawQuad(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& colour);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& scale, Ref<Texture2D> texture, const glm::vec4& tint = {1.0f, 1.0f, 1.0f, 1.0f});

		static void ResetStats();

		inline static Statistics* GetStats() { return m_Stats; };
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		static void Flush();

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		struct Storage
		{
			struct QuadVertex
			{
				float Position[3];
				float TexCoord[2];
				float TextureSlot;
				float Colour[4];
				float TilingFactor;
			};

			const uint32_t MaxQuads = 150000;
			const uint32_t MaxVertices = 4 * MaxQuads;
			const uint32_t MaxIndices = 6 * MaxQuads;
			const uint32_t MaxTextureSlots = 32;

			QuadVertex* QuadVerticesBase;
			QuadVertex* QuadVerticesPtr;

			Ref<VertexArray> QuadVertexArray;
			Ref<VertexBuffer> QuadVertexBuffer;
			Ref<IndexBuffer> QuadIndexBuffer;
			std::vector<Ref<Texture2D>> TextureSlots;
			uint32_t ReservedTexSlotsCount = 0;
			int32_t OccupiedTexSlotCount = 0; // -1 denotes that no slot is occupied
			uint32_t QuadCount = 0;

			Ref<Shader> TextureShader;
			Ref<Texture2D> WhiteTexture;
		};

		static SceneData* m_SceneData;
		static Storage* m_Storage;
		static Statistics* m_Stats;
	};
}
