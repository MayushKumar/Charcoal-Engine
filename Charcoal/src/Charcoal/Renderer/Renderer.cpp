#include "chpch.h"
#include "Renderer.h"

#include "RendererCommand.h"

namespace Charcoal
{

	void Renderer::BeginScene()
	{
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::weak_ptr<VertexArray>& vertexArray)
	{
		vertexArray.lock()->Bind();
		RendererCommand::DrawIndexed(vertexArray);
	}

}