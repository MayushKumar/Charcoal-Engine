#pragma once

#include "Charcoal/Core/Core.h"
#include "Charcoal/Renderer/VertexArray.h"
#include "Charcoal/Renderer/RendererAPI.h"

#include <glm/glm.hpp>

namespace Charcoal
{

	class RendererCommand
	{

	public:

		inline static void Init()
		{
			s_RendererAPI->Init();
		}

		inline static void SetClearColour(const glm::vec4& colour)
		{
			s_RendererAPI->SetClearColour(colour);
		}

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(x, y, width, height);
		}

		inline static void SetBlending(bool value)
		{
			s_RendererAPI->SetBlending(value);
		}

		inline static void SetPolygonMode(PolygonMode mode)
		{
			s_RendererAPI->SetPolygonMode(mode);
		}

		inline static void ClearColourBuffer()
		{
			s_RendererAPI->ClearColourBuffer();
		}

		inline static void ClearDepthBuffer()
		{
			s_RendererAPI->ClearDepthBuffer();
		}

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indicesCount = 0)
		{
			if(indicesCount == 0)
				s_RendererAPI->DrawIndexed(vertexArray, vertexArray->GetIndexBuffer()->GetCount());
			else
				s_RendererAPI->DrawIndexed(vertexArray, indicesCount);
		}

	private:
		static RendererAPI* s_RendererAPI;

	};

}
