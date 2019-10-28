#include <chpch.h>
#include "RendererCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Charcoal
{

	RendererAPI* RendererCommand::s_RendererAPI = new OpenGLRendererAPI();

	void RendererCommand::SetClearColour(const glm::vec4& colour)
	{
		s_RendererAPI->SetClearColour(colour);
	}

	void RendererCommand::Clear()
	{
		s_RendererAPI->Clear();
	}

	void RendererCommand::DrawIndexed(const Ref<VertexArray>& vertexArray)
	{
		s_RendererAPI->DrawIndexed(vertexArray);
	}

}