#include "chpch.h"
#include "OpenGLTexture.h"

#include "Charcoal/Core/Utilities.h"

#include <glad/glad.h>
#include <stb_image.h>
#include <glm/glm.hpp>

namespace Charcoal
{

	////////////////////  Texture2D  ////////////////////
	
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : m_Path(path)
	{
		CH_PROFILE_FUNCTION();

		ImageUtils::STB_Image image = ImageUtils::LoadImage(path, true);
		
		m_Width = image.Width;
		m_Height = image.Height;

		GLenum internalFormat = 0, dataFormat = 0;
		if (image.Channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		} else if (image.Channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}
		else CH_CORE_ASSERT(false, "Image not supported due to unsupported number of channels");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		glGenerateTextureMipmap(m_RendererID);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, image.Data);

		stbi_image_free(image.Data);
	}

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, void* data)
	{
		CH_PROFILE_FUNCTION();

		m_Width = width;
		m_Height = height;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, GL_RGBA8, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, data);
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

	
	////////////////////  CubeMap  ////////////////////

	OpenGLCubeMap::OpenGLCubeMap(const std::string& path)
	{
		CH_PROFILE_FUNCTION();
		
		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_RendererID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		static const char* fileNames[6] = {"px.png", "nx.png", "py.png", "ny.png", "pz.png", "nz.png"};
		
		for (uint32_t i = 0; i < 6; i++)
		{
			ImageUtils::STB_Image image = ImageUtils::LoadImage(path + fileNames[i], false);
			// glTextureStorage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 1, GL_RGBA8, image.Width, image.Height);
			// glTextureSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 0, 0, image.Width, image.Height, GL_RGBA, GL_UNSIGNED_BYTE, image.Data);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, image.Width, image.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.Data);
			stbi_image_free(image.Data);
		}
	}

	OpenGLCubeMap::~OpenGLCubeMap()
	{
		CH_PROFILE_FUNCTION();
		
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLCubeMap::Bind(uint32_t slot) const
	{
		CH_PROFILE_FUNCTION();
		
		glBindTextureUnit(slot, m_RendererID);
	}

}
