#pragma once

#include "Charcoal/Core/Core.h"
#include "Charcoal/Renderer/Texture.h"

namespace Charcoal
{

	struct FramebufferSpecifications
	{
		uint32_t Width;
		uint32_t Height;

		uint32_t ColourAttachmentCount = 1;
		
		bool IsSwapchainDefault = false;
	};
	
	class Framebuffer
	{
	  public:
		virtual ~Framebuffer() = default;
		
		static Ref<Framebuffer> Create(const FramebufferSpecifications& specs);

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetColourAttachment(uint32_t index) const = 0;
		virtual uint32_t GetDepthAttachment() const = 0;

		virtual const FramebufferSpecifications& GetSpecifications() const = 0;
	};
  
}
