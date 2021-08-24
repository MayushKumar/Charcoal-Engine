#pragma once

#include "Charcoal/Core/Core.h"
#include "Charcoal/Renderer/Texture.h"

#include <initializer_list>

namespace Charcoal
{

	enum class FramebufferTextureFormat
	{
		None = 0,
		RGBA,
		RGBA16F,
		Depth
	};

	struct FramebufferAttachmentSpecs
	{
		FramebufferTextureFormat Format;
	};

	struct FramebufferAttachment
	{
		FramebufferAttachment(FramebufferTextureFormat format) { m_Format = format; }
		// FramebufferAttachment(Ref<Texture2D> texture);
		// FramebufferAttachment(Ref<CubeMap> cubeMap, CubeMap::CubeMapFace face);

		uint32_t m_RendererID = 0;
		FramebufferTextureFormat m_Format;
	};

	struct FramebufferSpecs
	{
		uint32_t Width;
		uint32_t Height;

		std::vector<FramebufferAttachmentSpecs> AttachmentSpecs;

		bool IsSwapchainDefault = false;
	};

	class Framebuffer
	{
	  public:
		virtual ~Framebuffer() = default;
		
		static Ref<Framebuffer> Create(const FramebufferSpecs& specs);

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual void AttachTexture2D(Ref<Texture2D> texture, uint32_t attachmentID) = 0;
		virtual void AttachCubeMapFace(Ref<CubeMap> cubeMap, CubeMap::CubeMapFace face, uint32_t mipLevel, uint32_t attachmentID) = 0;

		virtual void BindColorAttachment(uint32_t index, uint32_t slot) = 0;
		virtual void BindDepthAttachment(uint32_t slot) = 0;

		virtual uint32_t GetColourAttachmentID(uint32_t index) const = 0;
		virtual uint32_t GetDepthAttachmentID() const = 0;

		virtual const FramebufferSpecs& GetSpecs() const = 0;
	};
  
	class FramebufferStack
	{
	public:
		FramebufferStack() = delete;

		static inline void Push(Ref<Framebuffer> framebuffer) { m_Stack.push_back(framebuffer); framebuffer->Bind(); }
		static inline void Pop() { m_Stack.pop_back(); m_Stack.size() == 0 ? m_DefaultSwapchainFramebuffer->Bind() : m_Stack.back()->Bind();}

		static inline Ref<Framebuffer> GetCurrentFramebuffer() { return m_Stack.size() == 0 ? m_DefaultSwapchainFramebuffer : m_Stack.back(); }

	private:
		static void UpdateDefaultSwapchainFramebufferSize(uint32_t width, uint32_t height) { m_DefaultSwapchainFramebuffer->Resize(width, height); }

	private:
		static std::vector<Ref<Framebuffer>> m_Stack;
		static Ref<Framebuffer> m_DefaultSwapchainFramebuffer;

		friend class Application;
	};
	
}
