#pragma once

#include "Charcoal/Renderer/RendererAPI.h"
#include "Charcoal/Renderer/VertexArray.h"
#include "Charcoal/Renderer/Shader.h"
#include "Charcoal/Renderer/Texture.h"
#include "Charcoal/Renderer/Framebuffer.h"
#include "Charcoal/Renderer/PerspectiveCamera.h"
#include "Charcoal/Renderer/Model.h"
#include "Charcoal/Renderer/Light.h"
#include "Charcoal/Renderer/SkyBox.h"
#include "Charcoal/Scene/Scene.h"

#include <glm/glm.hpp>

namespace Charcoal
{
	struct SceneData
	{
		SceneData() = default;

		PerspectiveCamera Camera;
		std::vector<PointLight> PointLights;
		Ref<SkyBox> Environment;
	};

	class Renderer3D
	{

	struct Storage
	{
		Ref<Framebuffer> HDRFramebuffer;
		Ref<VertexArray> TonemappingQuadVAO;
		Ref<Shader> TonemappingShader;

		Ref<Shader> SkyBoxShader;

		Ref<Texture2D> WhiteTexture;
		Ref<Texture2D> BlueTexture;
	};

	public:
		static void Init();
		static void ShutDown();

		static void BeginScene(const Ref<Scene3D> scene);
		static void EndScene();

		static void Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4 transform = glm::mat4(1.0f));
		static void Submit(const Ref<Model>& model, const Ref<Shader>& shader);
		static void Submit(const Ref<Model>& model, const Ref<VertexArray>& vertexArray, const Ref<PBRMaterial>& material);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	public:
		static Storage* m_Storage;
	private:
		static Ref<Scene3D> m_Scene;
	};

}
