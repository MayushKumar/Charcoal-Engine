#pragma once

#include "Charcoal/Renderer/RendererAPI.h"

namespace Charcoal
{

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void SetClearColour(const glm::vec4& colour) override;
		virtual void Clear() override;
		virtual void DrawIndexed(const std::weak_ptr<VertexArray>& vertexArray) override;
	};

}