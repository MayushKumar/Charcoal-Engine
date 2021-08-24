#include "chpch.h"
#include "OpenGLTexture.h"
#include "OpenGLShader.h"

#include "Charcoal/Core/Utilities.h"

#include <glad/glad.h>
#include <stb_image.h>
#include <glm/glm.hpp>

namespace Charcoal
{

	static GLenum TextureDataTypeToGLenum(TextureDataType type)
	{
		switch(type)
		{
		case TextureDataType::UByte: return GL_UNSIGNED_BYTE;
		case TextureDataType::Float: return GL_FLOAT;
		default: CH_CORE_ERROR("Invalid Texture Data Type!"); return 0;
		}
	}

	static GLenum TextureInternalFormatToGLenum(TextureInternalFormat format)
	{
		switch(format)
		{
		case TextureInternalFormat::R:        return GL_R8;
		case TextureInternalFormat::RG:       return GL_RG8;
		case TextureInternalFormat::RGB:      return GL_RGB8;
		case TextureInternalFormat::RGBA:     return GL_RGBA8;
		case TextureInternalFormat::R16F:     return GL_R16F;
		case TextureInternalFormat::RG16F:    return GL_RG16F;
		case TextureInternalFormat::RGB16F:   return GL_RGB16F;
		case TextureInternalFormat::RGBA16F:  return GL_RGBA16F;
		case TextureInternalFormat::R32F:     return GL_R32F;
		case TextureInternalFormat::RG32F:    return GL_RG32F;
		case TextureInternalFormat::RGB32F:   return GL_RGB32F;
		case TextureInternalFormat::RGBA32F:  return GL_RGBA32F;
		case TextureInternalFormat::Depth16:  return GL_DEPTH_COMPONENT16;
		case TextureInternalFormat::Depth32:  return GL_DEPTH_COMPONENT32;
		case TextureInternalFormat::Depth32F: return GL_DEPTH_COMPONENT32F;
		case TextureInternalFormat::Stencil:  return GL_STENCIL_INDEX8;
		case TextureInternalFormat::None:     CH_CORE_ERROR("Invalid Texture Internal Format!");
		}
		return 0;
	}

	static GLenum TextureFormatToGLenum(TextureFormat format)
	{
		switch(format)
		{
		case TextureFormat::R:         return GL_RED; 
		case TextureFormat::RG:        return GL_RG;
	    case TextureFormat::RGB:       return GL_RGB; 
		case TextureFormat::RGBA:      return GL_RGBA;
		case TextureFormat::Depth:     return GL_DEPTH;
		case TextureFormat::Stencil:   return GL_STENCIL;
		case TextureFormat::None:      CH_CORE_ERROR("Invalid Texture Internal Format!");
		}
	}

	static uint32_t TextureInternalFormatSize(TextureInternalFormat internalFormat)
	{
		return ((uint32_t)internalFormat / 4 + 1) * 8 * ((uint32_t)internalFormat % 4 + 1);
	}


	//______________________ Texture2D _______________________
	
	// OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : m_Path(path)
	// {
	// 	CH_PROFILE_FUNCTION();

	// 	ImageUtils::STB_Image image = ImageUtils::LoadImage(path.c_str(), true);
		
	// 	m_Width = image.Width;
	// 	m_Height = image.Height;

	// 	GLenum internalFormat = 0, dataFormat = 0;
	// 	if (image.Channels == 4)
	// 	{
	// 		internalFormat = GL_RGBA8;
	// 		dataFormat = GL_RGBA;
	// 	} else if (image.Channels == 3)
	// 	{
	// 		internalFormat = GL_RGB8;
	// 		dataFormat = GL_RGB;
	// 	}
	// 	else CH_CORE_ASSERT(false, "Image not supported due to unsupported number of channels");

	// 	glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
	// 	glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

	// 	glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 	glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// 	glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, image.Data);
	// 	glGenerateTextureMipmap(m_RendererID);

	// 	ImageUtils::FreeImageData(image);
	// }

	OpenGLTexture2D::OpenGLTexture2D(Texture2DSpec spec, const std::string& path)
	{
		CH_PROFILE_FUNCTION();
		m_Spec = spec;

		ImageUtils::STB_Image image = ImageUtils::LoadImage(path.c_str(), true);
		m_Width = image.Width;
		m_Height = image.Height;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, TextureInternalFormatToGLenum(m_Spec.InternalFormat), m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, TextureFormatToGLenum(m_Spec.Format),
							TextureDataTypeToGLenum(m_Spec.DataType), image.Data);
		
		glGenerateTextureMipmap(m_RendererID);

		ImageUtils::FreeImageData(image);
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
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}

	OpenGLTexture2D::OpenGLTexture2D(Texture2DSpec spec, uint32_t width, uint32_t height, void* data)
	{
		CH_PROFILE_FUNCTION();

		m_Spec = spec;
		m_Width = width;
		m_Height = height;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, spec.MipCount, TextureInternalFormatToGLenum(spec.InternalFormat),
						   m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		if(data)
			glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height,
								TextureFormatToGLenum(spec.Format), TextureDataTypeToGLenum(spec.DataType), data);
		
		glGenerateTextureMipmap(m_RendererID);
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

	uint32_t OpenGLTexture2D::GetDataSize() const
	{
		return m_Width * m_Height * TextureInternalFormatSize(m_Spec.InternalFormat);
	}

	void OpenGLTexture2D::GetData(void* location) const
	{
		glGetTextureImage(m_RendererID, 0, TextureFormatToGLenum(m_Spec.Format),
						  TextureDataTypeToGLenum(m_Spec.DataType), GetDataSize(), location);
	}

	
	//___________________ CubeMap ___________________

	OpenGLCubeMap::OpenGLCubeMap(uint32_t size, TextureInternalFormat internalFormat, uint32_t mipCount)
	{
		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_RendererID);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		// glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glTextureStorage2D(m_RendererID, mipCount, TextureInternalFormatToGLenum(internalFormat), size, size);
		// if (genMipmaps)
		// 	glGenerateTextureMipmap(m_RendererID);
	}

	OpenGLCubeMap::OpenGLCubeMap(const std::string& path)
	{
		CH_PROFILE_FUNCTION();

		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_RendererID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		static const char* fileNames[6] = {"px.png", "nx.png", "py.png", "ny.png", "pz.png", "nz.png"};
		
		for (uint32_t i = 0; i < 6; i++)
		{
			ImageUtils::STB_Image image = ImageUtils::LoadImage((path + fileNames[i]).c_str(), false);
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
