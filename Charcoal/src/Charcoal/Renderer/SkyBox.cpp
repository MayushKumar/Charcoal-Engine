#include "chpch.h"
#include "SkyBox.h"

#include "Charcoal/Core/Utilities.h"
#include "Charcoal/Renderer/ShaderManager.h"
#include "Charcoal/Renderer/Framebuffer.h"
#include "Charcoal/Renderer/RendererCommand.h"
#include "Charcoal/Renderer/TextureUtilities.h"
#include "Charcoal/Core/Application.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace Charcoal
{

	static Ref<Shader> s_irradianceConvolutionShader;
	static Ref<Shader> s_radiancePrefilterShader;
	static Ref<Shader> s_equiToCubeMapShader;
	static Ref<Framebuffer> s_captureFramebuffer;

	Ref<Texture2D> SkyBox::s_SpecularLUT;

	static void LoadOrPrefilterSpecularLUT();
	
	void SetupSkyBoxSystem()
	{
		s_equiToCubeMapShader = ShaderManager::LoadShader("assets/shaders/EquiToCube.glsl",
														  ShaderLanguage::GLSL);
		s_irradianceConvolutionShader = ShaderManager::LoadShader("assets/shaders/IrradianceConvolution.glsl",
																  ShaderLanguage::GLSL); 
		s_radiancePrefilterShader = ShaderManager::LoadShader("assets/shaders/RadiancePrefilter.glsl",
															  ShaderLanguage::GLSL);
		LoadOrPrefilterSpecularLUT();
	}

	static void RenderToCubeMap(Ref<CubeMap> cubeMap, Ref<Shader> shader, uint32_t size, uint32_t mipLevel);

	SkyBox::SkyBox(const std::string& equiMapPath)
	{
		Ref<Texture2D> equiMapTexture = Texture2D::Create({TextureInternalFormat::RGB16F, TextureFormat::RGB,
				TextureDataType::Float}, equiMapPath);
		uint32_t size = equiMapTexture->GetHeight();
		m_CubeMap = CubeMap::Create(size, TextureInternalFormat::RGBA);
		s_captureFramebuffer = Framebuffer::Create({size, size, {}});
		
		s_equiToCubeMapShader->Bind();
		s_equiToCubeMapShader->SetInt("equiMap", 0);
		equiMapTexture->Bind(0);
		
		RenderToCubeMap(m_CubeMap, s_equiToCubeMapShader, size, 0);

		GenerateIrradianceMap();
		PrefilterRadianceMap();
	}

	void SkyBox::GenerateIrradianceMap()
	{
		uint32_t size = m_CubeMap->GetSize();
		m_IrradianceMap = CubeMap::Create(size, TextureInternalFormat::RGBA);

		s_irradianceConvolutionShader->Bind();
		s_irradianceConvolutionShader->SetInt("envMap", 0);
		m_CubeMap->Bind(0);

		RenderToCubeMap(m_IrradianceMap, s_irradianceConvolutionShader, size, 0);
	}

	void SkyBox::PrefilterRadianceMap()
	{
		const uint32_t TOTAL_MIP_LEVELS = 5;
		uint32_t size = m_CubeMap->GetSize();
		m_RadianceMap = CubeMap::Create(size, TextureInternalFormat::RGBA, TOTAL_MIP_LEVELS);

		s_radiancePrefilterShader->Bind();
		s_radiancePrefilterShader->SetInt("envMap", 0);
		m_CubeMap->Bind(0);

		for (uint32_t i = 0; i < TOTAL_MIP_LEVELS; i++)
		{
			float roughness = (float)i / (float)(TOTAL_MIP_LEVELS - 1);
			s_radiancePrefilterShader->SetFloat("roughness", roughness);
			RenderToCubeMap(m_RadianceMap, s_radiancePrefilterShader, size, i);
		}
	}

	// uint32_t size : Size of original cubeMap, not the mipLevel
	static void RenderToCubeMap(Ref<CubeMap> cubeMap, Ref<Shader> shader, uint32_t size, uint32_t mipLevel)
	{
		uint32_t mipSize = size * std::pow(0.5f, mipLevel);
		s_captureFramebuffer->Resize(mipSize, mipSize);
		FramebufferStack::Push(s_captureFramebuffer);
		RendererCommand::SetViewport(0, 0, mipSize, mipSize);
		TextureUtilities::s_CubeVertexArray->Bind();

		glm::mat4 transforms[6] = {
				glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f),  glm::vec3(0.0f, -1.0f, 0.0f)),
				glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
				glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),  glm::vec3(0.0f, 0.0f, 1.0f)),
				glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
				glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, -1.0f, 0.0f)),
				glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))
		};

		shader->Bind();
		for(uint32_t i = 0; i < 6; i++)
		{
			shader->SetMat4("m_ViewProjection", transforms[i]);
			s_captureFramebuffer->AttachCubeMapFace(cubeMap, (CubeMap::CubeMapFace)i, mipLevel, 0);
			// RendererCommand::ClearColourBuffer();
			RendererCommand::DrawIndexed(TextureUtilities::s_CubeVertexArray);
		}

		RendererCommand::SetViewport(0, 0, Application::GetApplication().GetWindow().GetWidth(),
									 Application::GetApplication().GetWindow().GetHeight());
		FramebufferStack::Pop();
	}

	static void LoadOrPrefilterSpecularLUT()
	{
		const uint32_t LUT_SIZE = 512;

		Ref<Shader> shader = ShaderManager::LoadShader("assets/shaders/SpecularLUT.glsl", ShaderLanguage::GLSL);
		float vertices[4 * (2 + 2)] {
			//    Pos   |  TexCoords
			-1.0f, -1.0f, 0.0f, 0.0f,
			 1.0f, -1.0f, 1.0f, 0.0f, 
			 1.0f,  1.0f, 1.0f, 1.0f,
			-1.0f,  1.0f, 0.0f, 1.0f
		};

		uint32_t indices[2 * 3] {
			0, 1, 2,
			0, 2, 3
		};

		Ref<VertexArray> vertexArray = VertexArray::Create();
		BufferLayout layout = {
			{ShaderDataType::Float2, "a_Pos"},
			{ShaderDataType::Float2, "a_TexCoords"}
		};
		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(sizeof(vertices), vertices);
		vertexBuffer->SetLayout(layout);
		vertexArray->AddVertexBuffer(vertexBuffer);
		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(DataType::UInt32, sizeof(indices), indices);
		vertexArray->SetIndexBuffer(indexBuffer);

		SkyBox::s_SpecularLUT = Texture2D::Create({TextureInternalFormat::RG,
				TextureFormat::RG, TextureDataType::UByte}, LUT_SIZE, LUT_SIZE, nullptr);
		Ref<Framebuffer> framebuffer = Framebuffer::Create({LUT_SIZE, LUT_SIZE, {}});

		FramebufferStack::Push(framebuffer);
		RendererCommand::SetBlending(false);
		shader->Bind();
		RendererCommand::SetViewport(0, 0, LUT_SIZE, LUT_SIZE);
		framebuffer->AttachTexture2D(SkyBox::s_SpecularLUT, 0);
		RendererCommand::SetClearColour({0.0f, 1.0f, 0.0f, 1.0f});
		RendererCommand::ClearColourBuffer();
		vertexArray->Bind();
		RendererCommand::DrawIndexed(vertexArray);
		FramebufferStack::Pop();
		RendererCommand::SetViewport(0, 0, Application::GetApplication().GetWindow().GetWidth(),
									 Application::GetApplication().GetWindow().GetHeight());
		RendererCommand::SetBlending(true);

		std::vector<char> pixels = std::vector<char>(4 * 8 * LUT_SIZE * LUT_SIZE);
		SkyBox::s_SpecularLUT->GetData(pixels.data());
		ImageUtils::WritePNGImage("assets/textures/specular.png", true,
								  {LUT_SIZE, LUT_SIZE, 2, pixels.data()});

		// SkyBox::s_SpecularLUT = Texture2D::Create({TextureInternalFormat::RG, TextureFormat::RG, TextureDataType::UByte},
		// 										  "assets/textures/specular.png");
		// uint32_t test[10 * 10] = 
		// 	{
		// 		0xffffffff, 0x000000ff, 0xffffffff, 0x000000ff, 0xffffffff, 0x000000ff, 0xffffffff, 0x000000ff, 0xffffffff, 0x000000ff, 
		// 		0xffffffff, 0x000000ff, 0xffffffff, 0x000000ff, 0xffffffff, 0x000000ff, 0xffffffff, 0x000000ff, 0xffffffff, 0x000000ff, 
		// 		0xffffffff, 0x000000ff, 0xffffffff, 0x000000ff, 0xffffffff, 0x000000ff, 0xffffffff, 0x000000ff, 0xffffffff, 0x000000ff, 
		// 		0xffffffff, 0x000000ff, 0xffffffff, 0x000000ff, 0xffffffff, 0x000000ff, 0xffffffff, 0x000000ff, 0xffffffff, 0x000000ff, 
		// 		0xffffffff, 0x000000ff, 0xffffffff, 0x000000ff, 0xffffffff, 0x000000ff, 0xffffffff, 0x000000ff, 0xffffffff, 0x000000ff, 
		// 		0xffffffff, 0x000000ff, 0xffffffff, 0x000000ff, 0xffffffff, 0x000000ff, 0xffffffff, 0x000000ff, 0xffffffff, 0x000000ff, 
		// 		0xffffffff, 0x000000ff, 0xffffffff, 0x000000ff, 0xffffffff, 0x000000ff, 0xffffffff, 0x000000ff, 0xffffffff, 0x000000ff, 
		// 		0xffffffff, 0x000000ff, 0xffffffff, 0x000000ff, 0xffffffff, 0x000000ff, 0xffffffff, 0x000000ff, 0xffffffff, 0x000000ff, 
		// 		0xffffffff, 0x000000ff, 0xffffffff, 0x000000ff, 0xffffffff, 0x000000ff, 0xffffffff, 0x000000ff, 0xffffffff, 0x000000ff, 
		// 		0xffffffff, 0x000000ff, 0xffffffff, 0x000000ff, 0xffffffff, 0x000000ff, 0xffffffff, 0x000000ff, 0xffffffff, 0x000000ff
		// 	};
	}
	
	SkyBox::~SkyBox()
	{
		
	}
	
}
