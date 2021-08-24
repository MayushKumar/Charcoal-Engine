#include "chpch.h"
#include "Framebuffer.h"

#include "Charcoal/Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Charcoal
{
	static FramebufferTextureFormat TextureInternalFormatToFramebufferFormat(TextureInternalFormat format)
	{
		switch(format)
		{
		case TextureInternalFormat::RGBA:     return FramebufferTextureFormat::RGBA; break;
		case TextureInternalFormat::RGBA16F:  return FramebufferTextureFormat::RGBA16F; break;
		case TextureInternalFormat::Depth16:  return FramebufferTextureFormat::Depth; break;
		case TextureInternalFormat::Depth32:  return FramebufferTextureFormat::Depth; break;
		case TextureInternalFormat::Depth32F: return FramebufferTextureFormat::Depth; break;
		default:
			CH_CORE_ERROR("Invalid Texture Internal Format!");
			return FramebufferTextureFormat::None;
		}
	}

	// FramebufferAttachment::FramebufferAttachment(Ref<Texture2D> texture)
	// {
	// 	m_Format = TextureInternalFormatToFramebufferFormat(texture->GetInternalFormat());
	// }

	std::vector<Ref<Framebuffer>> FramebufferStack::m_Stack;
	Ref<Framebuffer> FramebufferStack::m_DefaultSwapchainFramebuffer = Framebuffer::Create({1280, 720, {}, true});

	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecs& specs)
	{
		switch (RendererAPI::GetAPI())
			{
			case RendererAPI::API::None: CH_CORE_ERROR("None is not a supported API type!");
			case RendererAPI::API::OpenGL: return CreateRef<OpenGLFramebuffer>(specs);
			}
		CH_CORE_ASSERT(false, "Not a recognized Renderer API");
		return Ref<Framebuffer>();
	}
	
}
