#include <chpch.h>
#include "Renderer.h"

#include "Charcoal/Core/Core.h"
#include "RendererCommand.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Charcoal
{

	/////////////////////////////  3D Renderer /////////////////////////////

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData();
	Renderer2D::SceneData* Renderer2D::m_SceneData = new Renderer2D::SceneData();
	Renderer2D::Renderer2DStorage* Renderer2D::m_Storage = new Renderer2D::Renderer2DStorage();

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

	/////////////////////////////  2D Renderer /////////////////////////////

	void Renderer2D::Init()
	{
		CH_PROFILE_FUNCTION();

		m_Storage->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		
		Ref<VertexArray> vertexArray = VertexArray::Create();
		BufferLayout layout(
			{
				{ShaderDataType::Float3, "a_Position"},
				{ShaderDataType::Float2, "a_TexCoords"}
			});

		float squareVertices[5 * 4] = {
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f
		};
		unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

		Ref<Charcoal::VertexBuffer> VB = VertexBuffer::Create(sizeof(squareVertices), squareVertices);
		Ref<Charcoal::IndexBuffer> IB = IndexBuffer::Create(sizeof(squareIndices), squareIndices);
		VB->SetLayout(layout);

		vertexArray->AddVertexBuffer(VB);
		vertexArray->SetIndexBuffer(IB);

		m_Storage->QuadVertexArray = vertexArray;

		m_Storage->WhiteTexture = Texture2D::Create(1, 1, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::ShutDown()
	{
		delete m_Storage;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& scale, const glm::vec4& colour)
	{
		DrawQuad(position, scale, m_Storage->WhiteTexture, colour);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& scale, Ref<Texture2D> texture, const glm::vec4& tint)
	{
		m_Storage->TextureShader->Bind();
		m_Storage->TextureShader->SetMat4("m_ViewProjection", m_SceneData->ViewProjectionMatrix);
		m_Storage->TextureShader->SetMat4("m_Transform", glm::scale(glm::mat4(1.0f), glm::vec3(scale.x, scale.y, 1.0f)));
		m_Storage->TextureShader->SetVec4("u_Colour", tint);
		m_Storage->QuadVertexArray->Bind();
		texture->Bind();
		RendererCommand::DrawIndexed(m_Storage->QuadVertexArray);
	}

}