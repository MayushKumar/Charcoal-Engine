#include <chpch.h>
#include "Renderer.h"

#include "Charcoal/Core.h"
#include "RendererCommand.h"

namespace Charcoal
{

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData();

	void Renderer::Init()
	{
		RendererCommand::Init();
	}

	void Renderer::BeginScene(const OrthographicCamera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4 transform)
	{
		shader->Bind();
		shader->SetMat4("m_ViewProjection", m_SceneData->ViewProjectionMatrix);
		shader->SetMat4("m_Transform", transform);
		vertexArray->Bind();
		RendererCommand::DrawIndexed(vertexArray);
	}

}