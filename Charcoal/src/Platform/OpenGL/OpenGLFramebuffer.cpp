#include "chpch.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace Charcoal
{

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecifications& specs)
		: m_Specifications(specs)
	{
		if (!m_Specifications.IsSwapchainDefault)
		{
			m_ColourAttachmentIDs.resize(specs.ColourAttachmentCount);
			for (uint32_t i = 0; i < m_Specifications.ColourAttachmentCount; i++)
			{
				glCreateFramebuffers(1, &m_RendererID);
				glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

				glCreateTextures(GL_TEXTURE_2D, 1, &m_ColourAttachmentIDs[i]);
				glBindTexture(GL_TEXTURE_2D, m_ColourAttachmentIDs[i]);
				glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, m_Specifications.Width, m_Specifications.Height);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_ColourAttachmentIDs[i], 0);
			}
			glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachmentID);
			glBindTexture(GL_TEXTURE_2D, m_DepthAttachmentID);
			glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, m_Specifications.Width, m_Specifications.Height);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachmentID, 0);

			CH_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is not complete!");
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		else
		{
			m_RendererID = 0;
		}
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		if (!m_Specifications.IsSwapchainDefault)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			for (uint32_t i = 0; i < m_Specifications.ColourAttachmentCount; i++)
			{
				glDeleteTextures(1, &m_ColourAttachmentIDs[i]);
			}
			glDeleteTextures(1, &m_DepthAttachmentID);			
		}
	}

	void OpenGLFramebuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
	}

	void OpenGLFramebuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
}
