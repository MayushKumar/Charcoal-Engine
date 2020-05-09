#include "chpch.h"
#include "OpenGLTexture.h"

#include <glad/glad.h>
#include <stb_image.h>
#include <glm/glm.hpp>

namespace Charcoal
{

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : m_Path(path)
	{
		CH_PROFILE_FUNCTION();

		int width, height, channels;
		stbi_set_flip_vertically_on_load(true);
		stbi_uc* data;
		{
			CH_PROFILE_SCOPE("OpenGLTexture2D : stbi_load");

			data = stbi_load(m_Path.c_str(), &width, &height, &channels, 0);
		}
		CH_CORE_ASSERT(data, "Failed to load image!");
		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		} else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}
		else CH_CORE_ASSERT(false, "Image not supported due to unsupported number of channels");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		glGenerateTextureMipmap(m_RendererID);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, const glm::vec4& colour)
	{
		CH_PROFILE_FUNCTION();

		unsigned int data = 0;
		data |= (int)((float)255 * colour.r);
		data <<= 8;
		data |= (int)((float)255 * colour.g);
		data <<= 8;
		data |= (int)((float)255 * colour.b);
		data <<= 8;
		data |= (int)((float)255 * colour.a);

		m_Width = width;
		m_Height = height;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, GL_RGBA8, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, &data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		CH_PROFILE_FUNCTION();

		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		CH_PROFILE_FUNCTION();

		glBindTextureUnit(slot, m_RendererID);
	}

}
