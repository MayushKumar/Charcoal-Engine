#pragma once

#include "chpch.h"

#include "VertexArray.h"

#include <glm/glm.hpp>

namespace Charcoal
{

	class RendererAPI
	{

	public:
		enum class API
		{
			None = 0, OpenGL
		};
		
	public:
		static inline API GetAPI() { return s_API; }

		virtual void SetClearColour(const glm::vec4& colour) = 0;
		virtual void Clear() = 0;
		virtual void DrawIndexed(const std::weak_ptr<VertexArray>& vertexArray) = 0;

	private:
		static API s_API;

	};

}