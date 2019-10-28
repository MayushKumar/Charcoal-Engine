#pragma once

#include "Charcoal/Core.h"
#include "Charcoal/Renderer/RendererAPI.h"

namespace Charcoal
{

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void SetClearColour(const glm::vec4& colour) override;
		virtual void Clear() override;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
	};

}