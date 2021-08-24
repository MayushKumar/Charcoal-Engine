#include <chpch.h>
#include "Texture.h"

#include "Charcoal/Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Charcoal
{

	//_______________Texture2D_________________

	Ref<Texture2D> Texture2D::Create(Texture2D::Texture2DSpec spec, uint32_t width, uint32_t height, void* data)
	{
		switch(RendererAPI::GetAPI())
		{
		case RendererAPI::API::None: CH_CORE_ERROR("None is not a supporter API type!");
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(spec, width, height, data);
		}
	}
	
	// Ref<Texture2D> Texture2D::Create(const std::string& path)
	// {
	// 	switch (RendererAPI::GetAPI())
	// 	{
	// 	case RendererAPI::API::None: CH_CORE_ERROR("None is not a supported API type!");
	// 	case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(path);
	// 	}
	// 	CH_CORE_ASSERT(false, "Not a recognized Renderer API");
	// 	return Ref<Texture2D>();
	// }

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

	Ref<Texture2D> Texture2D::Create(Texture2DSpec spec, const std::string& path)
	{
		switch(RendererAPI::GetAPI())
		{
		case RendererAPI::API::None: CH_CORE_ERROR("None is not a supported API type!");
		case RendererAPI::API::OpenGL:
			Ref<Texture2D> ref = CreateRef<OpenGLTexture2D>(spec, path);
			return ref;
		}
		return Ref<Texture2D>();
	}

	//_______________CubeMap________________

	Ref<CubeMap> CubeMap::Create(uint32_t size, TextureInternalFormat internalFormat, uint32_t mipCount)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None: CH_CORE_ERROR("None is not a supported API type!");
		case RendererAPI::API::OpenGL: {
			Ref<OpenGLCubeMap> ref = CreateRef<OpenGLCubeMap>(size, internalFormat, mipCount);
			ref->m_Size = size;
			return ref;
		}
		}
		CH_CORE_ASSERT(false, "Not a recognized Renderer API");
		return Ref<CubeMap>();
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
