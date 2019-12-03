#pragma once

#include "Charcoal/Core/Core.h"
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

		virtual void Init() = 0;
		virtual void SetClearColour(const glm::vec4& colour) = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void Clear() = 0;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;

	private:
		static API s_API;

	};

}