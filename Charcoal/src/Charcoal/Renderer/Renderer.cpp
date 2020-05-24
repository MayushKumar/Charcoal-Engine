#include <chpch.h>
#include "Renderer.h"

#include "Charcoal/Core/Core.h"
#include "RendererCommand.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

namespace Charcoal
{

	/////////////////////////////  3D Renderer /////////////////////////////

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData();

	void Renderer::Init()
	{
		RendererCommand::Init();
	}

	void Renderer::BeginScene(const PerspectiveCamera& camera)
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

	void Renderer::Submit(const Ref<Model>& model, const Ref<Shader>& shader, const glm::mat4 transform)
	{
		shader->Bind();
		shader->SetMat4("m_ViewProjection", m_SceneData->ViewProjectionMatrix);
		shader->SetMat4("m_Transform", transform);
		
		for (uint32_t i = 0; i < model->m_Meshes.size(); i++)
		{
			Ref<Material>& material = model->m_Materials[model->m_Meshes[i]->m_MaterialIndex];

			for (uint32_t j = 0; j < model->m_Materials[model->m_Meshes[i]->m_MaterialIndex]->m_Textures.size(); j++)
			{
				switch (model->m_Materials[model->m_Meshes[i]->m_MaterialIndex]->m_TextureTypes[j])
				{
				case TextureType::Albedo:
				{
					material->m_Textures[j]->Bind(0);
					shader->SetInt("material.AlbedoMap", 0);
					break;
				}
				case TextureType::Normal:
				{
					material->m_Textures[j]->Bind(1);
					shader->SetInt("material.NormalMap", 1);
					break;
				}
				case TextureType::Metallic:
				{
					material->m_Textures[j]->Bind(2);
					shader->SetInt("material.MetallicMap", 2);
					break;
				}
				case TextureType::Roughness:
				{
					material->m_Textures[j]->Bind(3);
					shader->SetInt("material.RoughnessMap", 3);
					break;
				}
				}
			}

			shader->SetVec3("material.AmbientColour", { material->m_AmbientColour[0], material->m_AmbientColour[1], material->m_AmbientColour[2] });
			shader->SetVec3("material.DiffuseColour", { material->m_DiffuseColour[0], material->m_DiffuseColour[1], material->m_DiffuseColour[2] });
			shader->SetFloat("material.MetallicFactor", material->m_MetallicFactor);
			shader->SetFloat("material.RoughnessFactor", material->m_RoughnessFactor);

			model->m_VertexArrays[i]->Bind();
			RendererCommand::DrawIndexed(model->m_VertexArrays[i]);
		}
	}

	/////////////////////////////  2D Renderer /////////////////////////////


	Renderer2D::SceneData* Renderer2D::m_SceneData = new Renderer2D::SceneData();
	Renderer2D::Storage* Renderer2D::m_Storage = new Renderer2D::Storage();
	Renderer2D::Statistics* Renderer2D::m_Stats = new Renderer2D::Statistics();

	void Renderer2D::Init()
	{
		CH_PROFILE_FUNCTION();

		m_Storage->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		m_Storage->WhiteTexture = Texture2D::Create(1, 1, { 1.0f, 1.0f, 1.0f, 1.0f });
		m_Storage->QuadVertexArray = VertexArray::Create();

		m_Storage->TextureShader->Bind();
		std::vector<int32_t> slots = std::vector<int32_t>(m_Storage->MaxTextureSlots);
		for (int32_t i = 0; i < m_Storage->MaxTextureSlots; i++)
			slots[i] = i;
		m_Storage->TextureShader->SetIntArray("u_TextureSlots", m_Storage->MaxTextureSlots, slots.data());

		BufferLayout layout = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float2, "a_TexCoord"},
			{ShaderDataType::Float, "a_TextureSlot"},
			{ShaderDataType::Float4, "a_Colour"},
			{ShaderDataType::Float, "a_TilingFactor"}
		};
		m_Storage->QuadVertexBuffer = VertexBuffer::Create(m_Storage->MaxVertices * layout.GetStride());
		m_Storage->QuadVertexBuffer->SetLayout(layout);
		m_Storage->QuadVertexArray->AddVertexBuffer(m_Storage->QuadVertexBuffer);

		uint32_t* indexData = new uint32_t[m_Storage->MaxIndices];
		uint32_t offset = 0;
		for(uint32_t i = 0; i < m_Storage->MaxIndices; i += 6)
		{
			indexData[  i  ] = 0 + offset;
			indexData[i + 1] = 1 + offset;
			indexData[i + 2] = 2 + offset;
			indexData[i + 3] = 2 + offset;
			indexData[i + 4] = 3 + offset;
			indexData[i + 5] = 0 + offset;

			offset += 4;
		}
		m_Storage->QuadIndexBuffer = IndexBuffer::Create(m_Storage->MaxIndices * sizeof(uint32_t), indexData);
		m_Storage->QuadVertexArray->SetIndexBuffer(m_Storage->QuadIndexBuffer);

		m_Storage->QuadVerticesBase = new Storage::QuadVertex[m_Storage->MaxVertices];
		m_Storage->QuadVerticesPtr = m_Storage->QuadVerticesBase;

		m_Storage->TextureSlots.resize(m_Storage->MaxTextureSlots); // TODO: Query from Renderer Capabilities
		m_Storage->TextureSlots[0] = m_Storage->WhiteTexture;
		m_Storage->ReservedTexSlotsCount++;
		m_Storage->OccupiedTexSlotCount = m_Storage->ReservedTexSlotsCount;
	}

	void Renderer2D::ShutDown()
	{
		delete m_SceneData;
		delete m_Storage;
		delete m_Stats;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
		m_Stats->MaxQuads = m_Storage->MaxQuads;
	}

	void Renderer2D::EndScene()
	{
		if (m_Storage->QuadCount > 0)
			Flush();
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& colour)
	{
		DrawQuad(position, scale, m_Storage->WhiteTexture, colour);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& scale, Ref<Texture2D> texture, const glm::vec4& tint)
	{
		if (m_Storage->QuadCount == m_Storage->MaxQuads) Flush();

		uint32_t textureSlot = 0;
		bool textureFound = false;
		for (uint32_t i = m_Storage->ReservedTexSlotsCount; i < m_Storage->OccupiedTexSlotCount; i++)
		{
			if (m_Storage->TextureSlots[i] == texture)
			{
				textureSlot = i;
				textureFound = true;
				break;
			}
		}
		if (!textureFound)
		{
			if(m_Storage->OccupiedTexSlotCount > (int32_t)m_Storage->MaxTextureSlots)
			{
				 Flush();
			}
				m_Storage->TextureSlots[m_Storage->OccupiedTexSlotCount] = texture;
				textureSlot = m_Storage->OccupiedTexSlotCount;
				m_Storage->OccupiedTexSlotCount++;
		}

		glm::mat4 transform = m_SceneData->ViewProjectionMatrix * glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(scale, 1.0f));
		glm::vec4 tempPos;

		tempPos = transform * glm::vec4(-0.5, -0.5f, 0, 1.0f);
		m_Storage->QuadVerticesPtr->Position[0] = tempPos.x;
		m_Storage->QuadVerticesPtr->Position[1] = tempPos.y;
		m_Storage->QuadVerticesPtr->Position[2] = tempPos.z;
		m_Storage->QuadVerticesPtr->TexCoord[0] = 0.0f;
		m_Storage->QuadVerticesPtr->TexCoord[1] = 0.0f;
		m_Storage->QuadVerticesPtr->TextureSlot = textureSlot;
		m_Storage->QuadVerticesPtr->Colour[0] = tint.r;
		m_Storage->QuadVerticesPtr->Colour[1] = tint.g;
		m_Storage->QuadVerticesPtr->Colour[2] = tint.b;
		m_Storage->QuadVerticesPtr->Colour[3] = tint.a;
		m_Storage->QuadVerticesPtr->TilingFactor = 1.0f;
		m_Storage->QuadVerticesPtr++;

		tempPos = transform * glm::vec4(0.5, -0.5f, 0, 1.0f);
		m_Storage->QuadVerticesPtr->Position[0] = tempPos.x;
		m_Storage->QuadVerticesPtr->Position[1] = tempPos.y;
		m_Storage->QuadVerticesPtr->Position[2] = tempPos.z;
		m_Storage->QuadVerticesPtr->TexCoord[0] = 1.0f;
		m_Storage->QuadVerticesPtr->TexCoord[1] = 0.0f;
		m_Storage->QuadVerticesPtr->TextureSlot = textureSlot;
		m_Storage->QuadVerticesPtr->Colour[0] = tint.r;
		m_Storage->QuadVerticesPtr->Colour[1] = tint.g;
		m_Storage->QuadVerticesPtr->Colour[2] = tint.b;
		m_Storage->QuadVerticesPtr->Colour[3] = tint.a;
		m_Storage->QuadVerticesPtr->TilingFactor = 1.0f;
		m_Storage->QuadVerticesPtr++;

		tempPos = transform * glm::vec4(0.5, 0.5f, 0, 1.0f);
		m_Storage->QuadVerticesPtr->Position[0] = tempPos.x;
		m_Storage->QuadVerticesPtr->Position[1] = tempPos.y;
		m_Storage->QuadVerticesPtr->Position[2] = tempPos.z;
		m_Storage->QuadVerticesPtr->TexCoord[0] = 1.0f;
		m_Storage->QuadVerticesPtr->TexCoord[1] = 1.0f;
		m_Storage->QuadVerticesPtr->TextureSlot = textureSlot;
		m_Storage->QuadVerticesPtr->Colour[0] = tint.r;
		m_Storage->QuadVerticesPtr->Colour[1] = tint.g;
		m_Storage->QuadVerticesPtr->Colour[2] = tint.b;
		m_Storage->QuadVerticesPtr->Colour[3] = tint.a;
		m_Storage->QuadVerticesPtr->TilingFactor = 1.0f;
		m_Storage->QuadVerticesPtr++;

		tempPos = transform * glm::vec4(-0.5, 0.5f, 0, 1.0f);
		m_Storage->QuadVerticesPtr->Position[0] = tempPos.x;
		m_Storage->QuadVerticesPtr->Position[1] = tempPos.y;
		m_Storage->QuadVerticesPtr->Position[2] = tempPos.z;
		m_Storage->QuadVerticesPtr->TexCoord[0] = 0.0f;
		m_Storage->QuadVerticesPtr->TexCoord[1] = 1.0f;
		m_Storage->QuadVerticesPtr->TextureSlot = textureSlot;
		m_Storage->QuadVerticesPtr->Colour[0] = tint.r;
		m_Storage->QuadVerticesPtr->Colour[1] = tint.g;
		m_Storage->QuadVerticesPtr->Colour[2] = tint.b;
		m_Storage->QuadVerticesPtr->Colour[3] = tint.a;
		m_Storage->QuadVerticesPtr->TilingFactor = 1.0f;
		m_Storage->QuadVerticesPtr++;

		m_Storage->QuadCount++;
		m_Stats->QuadCount++;
	}

	void Renderer2D::ResetStats()
	{
		m_Stats->QuadCount = 0;
		m_Stats->DrawCalls = 0;
	}

	void Renderer2D::Flush()
	{
		m_Storage->QuadVertexBuffer->SetData((float*)m_Storage->QuadVerticesBase, m_Storage->QuadVertexBuffer->GetLayout().GetStride() * m_Storage->QuadCount * 4);

		m_Storage->TextureShader->Bind();
		for (int32_t i = 0; i < m_Storage->OccupiedTexSlotCount; i++)
		{
			m_Storage->TextureSlots[i]->Bind(i);
		}
		m_Storage->QuadVertexArray->Bind();
		RendererCommand::DrawIndexed(m_Storage->QuadVertexArray, m_Storage->QuadCount * 6);

		m_Stats->DrawCalls++;

		m_Storage->QuadCount = 0;
		m_Storage->OccupiedTexSlotCount = m_Storage->ReservedTexSlotsCount;
		m_Storage->QuadVerticesPtr = m_Storage->QuadVerticesBase;
	}

}