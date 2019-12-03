#pragma once

#include "Charcoal/Core/Core.h"
#include "VertexArray.h"
#include "RendererAPI.h"

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

		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawIndexed(vertexArray);
		}

	private:
		static RendererAPI* s_RendererAPI;

	};

}