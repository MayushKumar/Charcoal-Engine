#include <chpch.h>
#include "Texture.h"

#include "Charcoal/Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Charcoal
{

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None: CH_CORE_ERROR("None is not a supported API type!");
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(path);
		}
		CH_CORE_ASSERT(false, "Not a recognized Renderer API");
		return Ref<Texture2D>();
	}

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height, void* data)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None: CH_CORE_ERROR("None is not a supported API type!");
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(width, height, data);
		}
		CH_CORE_ASSERT(false, "Not a recognized Renderer API");
		return Ref<Texture2D>();
	}

	Ref<CubeMap> CubeMap::Create(const std::string& path)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None: CH_CORE_ERROR("None is not a supported API type!");
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLCubeMap>(path);
		}
		CH_CORE_ASSERT(false, "Not a recognized Renderer API");
		return Ref<CubeMap>();
	}


}
