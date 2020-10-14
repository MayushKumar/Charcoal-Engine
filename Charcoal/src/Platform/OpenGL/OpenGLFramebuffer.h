#pragma once

#include "Charcoal/Renderer/Framebuffer.h"

namespace Charcoal
{

	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecifications& specs);
		virtual ~OpenGLFramebuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;
		
		virtual inline uint32_t GetColourAttachment(uint32_t index) const override { return m_ColourAttachmentIDs[index]; }
		virtual inline uint32_t GetDepthAttachment() const override { return m_DepthAttachmentID; }

		virtual const inline FramebufferSpecifications& GetSpecifications() const override { return m_Specifications; }
		
	private:
		uint32_t m_RendererID;
		std::vector<uint32_t> m_ColourAttachmentIDs;
		uint32_t m_DepthAttachmentID;

		FramebufferSpecifications m_Specifications;
	};
	
}
