#include <chpch.h>
#include "Renderer3D.h"

#include "Charcoal/Core/Core.h"
#include "RendererCommand.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

namespace Charcoal
{

	/////////////////////////////  3D Renderer /////////////////////////////

	Ref<Scene3D> Renderer3D::m_Scene;
	
	void Renderer3D::Init()
	{
		CH_PROFILE_FUNCTION();
	}

	void Renderer3D::ShutDown()
	{
	}

	void Renderer3D::BeginScene(const Ref<Scene3D> scene)
	{
		CH_PROFILE_FUNCTION();
		m_Scene = scene;
	}

	void Renderer3D::EndScene()
	{
		CH_PROFILE_FUNCTION();
	}

	void Renderer3D::Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4 transform)
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
	
	void Renderer3D::Submit(const Ref<Model>& model, const Ref<VertexArray>& vertexArray, const Ref<PBRMaterial>& material)
	{
		
		Ref<Shader>& shader = material->m_Shader;
		shader->Bind();
		shader->SetMat4("m_ViewProjection", m_Scene->m_Camera.GetViewProjectionMatrix());
		shader->SetMat4("m_Transform", model->m_Transform);
		shader->SetMat3("m_NormalMatrix", model->m_NormalMatrix);
		shader->SetVec3("ViewPos", m_Scene->m_Camera.GetPostion());
		shader->SetInt("pointLightCount", m_Scene->m_PointLights.size());

		if (m_Scene->IsSkyBoxEnabled())
		{
			m_Scene->GetSkyBox()->GetCubeMap()->Bind(0);
			shader->SetInt("cubeMap", 0);
		}
		
		for (uint32_t i = 0; i < m_Scene->m_PointLights.size(); i++)
		{
			std::string index = std::to_string(i);
			shader->SetVec3(("pointLights[" + index + "].Pos").c_str(), m_Scene->m_PointLights[i].Position);
			shader->SetVec3(("pointLights[" + index + "].Colour").c_str(), m_Scene->m_PointLights[i].Colour);
			shader->SetFloat(("pointLights[" + index + "].LuminousPower").c_str(), m_Scene->m_PointLights[i].LuminousPower);
		}
		material->m_AlbedoMap->Bind(1);
		material->m_NormalMap->Bind(2);
		material->m_MetallicMap->Bind(3);
		material->m_RoughnessMap->Bind(4);
		shader->SetInt("material.AlbedoMap", 1);
		shader->SetInt("material.NormalMap", 2);
		shader->SetInt("material.MetallicMap", 3);
		shader->SetInt("material.RoughnessMap", 4);
		shader->SetVec3("material.AmbientColour", { material->m_AmbientColour[0], material->m_AmbientColour[1], material->m_AmbientColour[2] });
		shader->SetVec3("material.DiffuseColour", { material->m_DiffuseColour[0], material->m_DiffuseColour[1], material->m_DiffuseColour[2] });
		shader->SetFloat("material.MetallicFactor", material->m_MetallicFactor);
		shader->SetFloat("material.RoughnessFactor", material->m_RoughnessFactor);

		vertexArray->Bind();
		RendererCommand::DrawIndexed(vertexArray);
	}

}

