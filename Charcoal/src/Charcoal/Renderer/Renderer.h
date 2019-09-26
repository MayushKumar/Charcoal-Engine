#pragma once

#include "VertexArray.h"
#include "RendererAPI.h"

namespace Charcoal
{

	class Renderer
	{
	public:
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

		static void BeginScene();
		static void EndScene();

		static void Submit(const std::weak_ptr<VertexArray>& vertexArray);
	};

}