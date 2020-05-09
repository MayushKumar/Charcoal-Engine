#include <chpch.h>
#include "OpenGLRendererAPI.h"

#include "Charcoal/Renderer/RendererCommand.h"

#include <glad/glad.h>

namespace Charcoal
{

	void OpenGLRendererAPI::Init()
	{
		CH_PROFILE_FUNCTION();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void OpenGLRendererAPI::SetClearColour(const glm::vec4& colour)
	{
		CH_PROFILE_FUNCTION();

		glClearColor(colour.r, colour.g, colour.b, colour.a);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		CH_PROFILE_FUNCTION();

		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetPolygonMode(PolygonMode mode)
	{
		switch (mode)
		{
		case PolygonMode::Fill:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		case PolygonMode::Lines:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		case PolygonMode::Points:
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			break;
		}
	}

	void OpenGLRendererAPI::Clear()
	{
		CH_PROFILE_FUNCTION();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
	{
		CH_PROFILE_FUNCTION();

		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

}
