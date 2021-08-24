#include "Charcoal/Renderer/Framebuffer.h"
#include "Charcoal/Renderer/Texture.h"
#include "chpch.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>
#include <initializer_list>

namespace Charcoal
{

	static GLenum FramebufferTextureFormatToGLenum(FramebufferTextureFormat format)
	{
		switch(format)
		{
		case FramebufferTextureFormat::RGBA    : return GL_RGBA8; break;
		case FramebufferTextureFormat::RGBA16F : return GL_RGBA16F; break;
		case FramebufferTextureFormat::Depth   : return GL_DEPTH_COMPONENT32; break;
		case FramebufferTextureFormat::None    : return 0; CH_CORE_ERROR("Invalid Framebuffer Texture Format!");
		}
	}

	static bool IsDepthFormat(FramebufferTextureFormat format)
	{
		switch(format)
		{
		case FramebufferTextureFormat::Depth : return true; break;
		default: return false;
		}
	}

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecs& specs)
		: m_Specs(specs)
	{
		if(!m_Specs.IsSwapchainDefault)
		{
			glCreateFramebuffers(1, &m_RendererID);
			if(!m_Specs.AttachmentSpecs.empty())
			{
				glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
				for(FramebufferAttachmentSpecs& spec :  m_Specs.AttachmentSpecs)
				{
					if(IsDepthFormat(spec.Format))
					{
						m_HasDepthAttachment = true;
						m_DepthAttachmentSpec = spec;
					}
					else
						m_ColourAttachmentSpecs.push_back(spec);
				}
				CreateAttachments();

				CH_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
							   "Framebuffer is not complete!");
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}
		}
		else
			m_RendererID = 0;
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		if (!m_Specs.IsSwapchainDefault)
			DeleteAttachments();
		glDeleteFramebuffers(1, &m_RendererID);
	}

	void OpenGLFramebuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
	}

	void OpenGLFramebuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		if (width == m_Specs.Width && height == m_Specs.Height)
			return;

		m_Specs.Width = width;
		m_Specs.Height = height;
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID); // TODO: Unbind at the end of the function maybe?
		if(!m_Specs.IsSwapchainDefault)
		{
			DeleteAttachments();
			CreateAttachments();
		}
	}

	void OpenGLFramebuffer::BindColorAttachment(uint32_t index, uint32_t slot)
	{
		glBindTextureUnit(slot, m_ColourAttachmentIDs[index]);
	}

	void OpenGLFramebuffer::BindDepthAttachment(uint32_t slot)
	{
		glBindTextureUnit(slot, m_DepthAttachmentID);
	}
	
    // Warning: Unbinds the last bound framebuffer
	void OpenGLFramebuffer::AttachTexture2D(Ref<Texture2D> texture, uint32_t attachmentID) 
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachmentID,
							   GL_TEXTURE_2D, texture->GetRendererID(), 0);
		glDrawBuffer(GL_COLOR_ATTACHMENT0 + attachmentID);
	}

	// Warning: Unbinds the last bound framebuffer
	void OpenGLFramebuffer::AttachCubeMapFace(Ref<CubeMap> cubeMap, CubeMap::CubeMapFace face, uint32_t mipLevel, uint32_t attachmentID) 
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachmentID,
							   GL_TEXTURE_CUBE_MAP_POSITIVE_X + (uint32_t)face, cubeMap->GetRendererID(), mipLevel);
		glDrawBuffer(GL_COLOR_ATTACHMENT0 + attachmentID);
	}

	void OpenGLFramebuffer::SetupColourAttachment(uint32_t index, FramebufferTextureFormat format)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_ColourAttachmentIDs[index]);
		glBindTexture(GL_TEXTURE_2D, m_ColourAttachmentIDs[index]);
		glTexStorage2D(GL_TEXTURE_2D, 1, FramebufferTextureFormatToGLenum(format),
					   m_Specs.Width, m_Specs.Height);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index,
							   GL_TEXTURE_2D, m_ColourAttachmentIDs[index], 0);
	}

	void OpenGLFramebuffer::SetupDepthAttachment(FramebufferTextureFormat format)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachmentID);
		glBindTexture(GL_TEXTURE_2D, m_DepthAttachmentID);
		glTexStorage2D(GL_TEXTURE_2D, 1, FramebufferTextureFormatToGLenum(format),
					   m_Specs.Width, m_Specs.Height);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachmentID, 0);
	}

	void OpenGLFramebuffer::CreateAttachments()
	{
		m_ColourAttachmentIDs.resize(m_ColourAttachmentSpecs.size());
		if(!(m_ColourAttachmentIDs.size() == 0))
		{
		for(uint32_t i = 0; i < m_ColourAttachmentSpecs.size(); i++)
			SetupColourAttachment(i, m_ColourAttachmentSpecs[i].Format);
		}
		if(m_HasDepthAttachment)
			SetupDepthAttachment(m_DepthAttachmentSpec.Format);
	}

	void OpenGLFramebuffer::DeleteAttachments()
	{
		glDeleteTextures(m_ColourAttachmentIDs.size(), m_ColourAttachmentIDs.data());
		if(m_HasDepthAttachment)
			glDeleteTextures(1, &m_DepthAttachmentID);
	}
	
}
