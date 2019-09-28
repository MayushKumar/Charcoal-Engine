#include "chpch.h"
#include "Renderer.h"

#include "RendererCommand.h"

namespace Charcoal
{

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData();

	void Renderer::BeginScene(const OrthographicCamera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::weak_ptr<VertexArray>& vertexArray, const std::weak_ptr<Shader>& shader)
	{
		shader.lock()->Bind();
		shader.lock()->SetMat4("m_ViewProjection", m_SceneData->ViewProjectionMatrix);
		vertexArray.lock()->Bind();
		RendererCommand::DrawIndexed(vertexArray);
	}

}