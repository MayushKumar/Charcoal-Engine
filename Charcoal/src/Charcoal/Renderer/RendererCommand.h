#pragma once

#include "chpch.h"

#include "VertexArray.h"
#include "RendererAPI.h"

namespace Charcoal
{

	class RendererCommand
	{

	public:
		static void SetClearColour(const glm::vec4& colour);
		static void Clear();
		static void DrawIndexed(const std::weak_ptr<VertexArray>& vertexArray);

	private:
		static RendererAPI* s_RendererAPI;

	};

}