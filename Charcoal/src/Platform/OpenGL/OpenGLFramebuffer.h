#pragma once

#include "Charcoal/Renderer/Framebuffer.h"
#include <initializer_list>

namespace Charcoal
{

	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecs& specs);

		virtual ~OpenGLFramebuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void Resize(uint32_t width , uint32_t height) override;
		
		virtual void BindColorAttachment(uint32_t index, uint32_t slot) override;
		virtual void BindDepthAttachment(uint32_t slot) override;

		virtual inline uint32_t GetColourAttachmentID(uint32_t index) const override { return m_ColourAttachmentIDs[index]; }
		virtual inline uint32_t GetDepthAttachmentID() const override { return m_DepthAttachmentID; }

		virtual void AttachTexture2D(Ref<Texture2D> texture, uint32_t attachmentID) override;
		virtual void AttachCubeMapFace(Ref<CubeMap> cubeMap, CubeMap::CubeMapFace face, uint32_t mipLevel, uint32_t attachmentID) override;

		virtual const inline FramebufferSpecs& GetSpecs() const override { return m_Specs; }

	private:
		void CreateAttachments();
		void DeleteAttachments();
		void SetupColourAttachment(uint32_t index, FramebufferTextureFormat format);
		void SetupDepthAttachment(FramebufferTextureFormat format);

	private:
		uint32_t m_RendererID;

		std::vector<uint32_t> m_ColourAttachmentIDs;
		std::vector<FramebufferAttachmentSpecs> m_ColourAttachmentSpecs;

		uint32_t m_DepthAttachmentID;
		FramebufferAttachmentSpecs m_DepthAttachmentSpec;
		bool m_HasDepthAttachment = false;

		FramebufferSpecs m_Specs;
	};
	
}
