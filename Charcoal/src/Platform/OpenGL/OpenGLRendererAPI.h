#pragma once

#include "Charcoal/Core/Core.h"
#include "Charcoal/Renderer/RendererAPI.h"

namespace Charcoal
{

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void SetClearColour(const glm::vec4& colour) override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void SetPolygonMode(PolygonMode mode) override;
		virtual void ClearColourBuffer() override;
		virtual void ClearDepthBuffer() override;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indicesCount) override;
	};

}