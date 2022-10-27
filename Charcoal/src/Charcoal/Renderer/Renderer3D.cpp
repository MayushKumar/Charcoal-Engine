#include <chpch.h>
#include "Renderer3D.h"

#include "Charcoal/Renderer/Buffer.h"
#include "Charcoal/Renderer/Framebuffer.h"
#include "Charcoal/Renderer/SkyBox.h"
#include "Charcoal/Renderer/VertexArray.h"
#include "Charcoal/Renderer/RendererCommand.h"
#include "Charcoal/Renderer/ShaderManager.h"
#include "Charcoal/Renderer/TextureUtilities.h"

#include "Charcoal/Core/Core.h"
#include "Charcoal/Core/Application.h"

#include "Charcoal/Renderer/SystemInitDefs.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Charcoal
{

	Ref<Scene3D> Renderer3D::m_Scene;
	Renderer3D::Storage* Renderer3D::m_Storage = new Renderer3D::Storage;
	
	void Renderer3D::Init()
	{
		CH_PROFILE_FUNCTION();
		{
			TextureUtilities::InitTextureUtilities();

			//HDR Initialization
			m_Storage->HDRFramebuffer = Framebuffer::Create({
					Application::GetApplication().GetWindow().GetWidth(),
					Application::GetApplication().GetWindow().GetHeight(),
					{{FramebufferTextureFormat::RGBA16F}, {FramebufferTextureFormat::Depth}}
					// {{FramebufferTextureFormat::RGBA}}
				});

		    float vertices[4 * (2 + 2)] = {
				 1.0f,  1.0f, 1.0f, 1.0f,
				-1.0f, -1.0f, 0.0f, 0.0f,
				 1.0f, -1.0f, 1.0f, 0.0f,
				-1.0f,  1.0f, 0.0f, 1.0f
			};

			uint32_t indices[4 * 2] = {
				0, 1, 2,
				0, 3, 1
			};

		    BufferLayout layout = {
				{ShaderDataType::Float2, "a_Pos"},
				{ShaderDataType::Float2, "a_TexCoords"}
			};

			m_Storage->TonemappingQuadVAO = VertexArray::Create();
			m_Storage->TonemappingQuadVAO->Bind();

			Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(sizeof(vertices), vertices);
			vertexBuffer->Bind();
			vertexBuffer->SetLayout(layout);
			Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(DataType::UInt32, sizeof(indices), indices);
			indexBuffer->Bind();

			m_Storage->TonemappingQuadVAO->AddVertexBuffer(vertexBuffer);
			m_Storage->TonemappingQuadVAO->SetIndexBuffer(indexBuffer);

			m_Storage->TonemappingShader = ShaderManager::LoadShader("assets/shaders/Tonemapping.glsl",
																	 ShaderLanguage::GLSL);
		}

		// Random Stuff
		m_Storage->SkyBoxShader = ShaderManager::LoadShader("assets/shaders/SkyBox.glsl",
															ShaderLanguage::GLSL);
		SetupSkyBoxSystem();
	}

	void Renderer3D::ShutDown()
	{
		delete m_Storage;
	}

	void Renderer3D::BeginScene(const Ref<Scene3D> scene)
	{
		CH_PROFILE_FUNCTION();
		m_Scene = scene;
		
		Ref<Framebuffer> currentFramebuffer = FramebufferStack::GetCurrentFramebuffer();
		m_Storage->HDRFramebuffer->Resize(currentFramebuffer->GetSpecs().Width,
										  currentFramebuffer->GetSpecs().Height);
		FramebufferStack::Push(m_Storage->HDRFramebuffer);
		RendererCommand::SetClearColour({0.0f, 0.0f, 0.1f, 1.0f});
		RendererCommand::ClearColourBuffer();
		RendererCommand::ClearDepthBuffer();
	}

	void Renderer3D::EndScene()
	{
		CH_PROFILE_FUNCTION();

		if (m_Scene->IsSkyBoxEnabled())
		{
			m_Storage->SkyBoxShader->Bind();
			m_Storage->SkyBoxShader->SetMat4("m_ViewProjection", m_Scene->m_Camera.GetProjectionMatrix()
											 * glm::mat4(glm::mat3(m_Scene->m_Camera.GetViewMatrix())));
			// m_Scene->GetSkyBox()->m_CubeMap->Bind(0);
			m_Scene->GetSkyBox()->m_CubeMap->Bind(0);
			m_Storage->SkyBoxShader->SetInt("cubeMap", 0);
			TextureUtilities::s_CubeVertexArray->Bind();
			RendererCommand::DrawIndexed(TextureUtilities::s_CubeVertexArray);
		}

		//HDR
		FramebufferStack::Pop();
		m_Storage->TonemappingShader->Bind();
		m_Storage->HDRFramebuffer->BindColorAttachment(0, 0);
		// SkyBox::s_SpecularLUT->Bind();
		m_Storage->TonemappingShader->SetInt("u_HDRTexture", 0);
		m_Storage->TonemappingQuadVAO->Bind();
		RendererCommand::DrawIndexed(m_Storage->TonemappingQuadVAO);
	}

	void Renderer3D::Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader,
							const glm::mat4 transform)
	{
		CH_PROFILE_FUNCTION();

		shader->Bind();
		shader->SetMat4("m_ViewProjection", m_Scene->m_Camera.GetViewProjectionMatrix());
		shader->SetMat4("m_Transform", transform);
		vertexArray->Bind();
		RendererCommand::DrawIndexed(vertexArray);
	}

	void Renderer3D::Submit(const Ref<Model>& model, const Ref<Shader>& shader)
	{	
		for (uint32_t i = 0; i < model->m_Meshes.size(); i++)
		{
			Submit(model, model->m_VertexArrays[i], model->m_Materials[i]);
		}
	}
	
	void Renderer3D::Submit(const Ref<Model>& model, const Ref<VertexArray>& vertexArray,
							const Ref<PBRMaterial>& material)
	{
		Ref<Shader>& shader = material->m_Shader;
		shader->Bind();
		shader->SetMat4("m_ViewProjection", m_Scene->m_Camera.GetViewProjectionMatrix());
		shader->SetMat4("m_Transform", model->m_Transform);
		shader->SetMat3("m_NormalMatrix", model->m_NormalMatrix);
		shader->SetVec3("ViewPos", m_Scene->m_Camera.GetPostion());
		shader->SetInt("pointLightCount", m_Scene->m_PointLights.size());

		material->m_AlbedoMap->Bind(0);
		material->m_NormalMap->Bind(1);
		material->m_MetallicRoughnessMap->Bind(2);
		shader->SetInt("material.AlbedoMap", 0);
		shader->SetInt("material.NormalMap", 1);
		shader->SetInt("material.MetallicRoughnessMap", 2);
		shader->SetVec4("material.BaseColour", { material->m_BaseColour[0],
				material->m_BaseColour[1], material->m_BaseColour[2], material->m_BaseColour[3] });
		shader->SetFloat("material.MetallicFactor", material->m_MetallicFactor);
		shader->SetFloat("material.RoughnessFactor", material->m_RoughnessFactor);

		if (m_Scene->IsSkyBoxEnabled())
		{
			m_Scene->GetSkyBox()->m_IrradianceMap->Bind(4);
			m_Scene->GetSkyBox()->m_RadianceMap->Bind(5);
			m_Scene->GetSkyBox()->s_SpecularLUT->Bind(6);
			shader->SetInt("irradianceMap", 4);
			shader->SetInt("radianceMap", 5);
			shader->SetInt("specularLUT", 6);
		}
		
		for (uint32_t i = 0; i < m_Scene->m_PointLights.size(); i++)
		{
			std::string index = std::to_string(i);
			shader->SetVec3(("pointLights[" + index + "].Pos").c_str(),
							m_Scene->m_PointLights[i].Position);
			shader->SetVec3(("pointLights[" + index + "].Colour").c_str(),
							m_Scene->m_PointLights[i].Colour);
			shader->SetFloat(("pointLights[" + index + "].LuminousPower").c_str(),
							 m_Scene->m_PointLights[i].LuminousPower);
		}

		vertexArray->Bind();
		RendererCommand::DrawIndexed(vertexArray);
	}

}

