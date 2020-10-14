#include "chpch.h"
#include "Framebuffer.h"

#include "Charcoal/Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Charcoal
{

	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecifications& specs)
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
