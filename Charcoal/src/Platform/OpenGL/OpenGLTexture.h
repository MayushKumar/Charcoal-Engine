#pragma once

#include "Charcoal/Renderer/Texture.h"

namespace Charcoal
{

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(Texture2DSpec spec, uint32_t width, uint32_t height, void* data);
		OpenGLTexture2D(Texture2DSpec spec, const std::string& path);
		OpenGLTexture2D(uint32_t width, uint32_t height, void* data);
		virtual ~OpenGLTexture2D();

		virtual void Bind(uint32_t slot = 0) const override;
		virtual uint32_t GetDataSize() const override;
		virtual void GetData(void* location) const override;

		virtual uint32_t GetRendererID() const override { return m_RendererID; }

	private:
		std::string m_Path;
		uint32_t m_RendererID;
	};

	class OpenGLCubeMap : public CubeMap
	{
	public:
		OpenGLCubeMap(uint32_t size, TextureInternalFormat internalFormat, uint32_t mipCount);
		OpenGLCubeMap(const std::string& path);
		virtual ~OpenGLCubeMap();

		virtual void Bind(uint32_t slot = 0) const override;
		virtual uint32_t GetRendererID() const override { return m_RendererID; }

	private:
		std::string m_Path;
		uint32_t m_RendererID;
	};

}
