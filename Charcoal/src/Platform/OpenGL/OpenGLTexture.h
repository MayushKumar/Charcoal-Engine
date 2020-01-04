#pragma once

#include "Charcoal/Renderer/Texture.h"

namespace Charcoal
{

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& path);
		OpenGLTexture2D(uint32_t width, uint32_t height, const glm::vec4& colour);
		virtual ~OpenGLTexture2D();

		virtual inline uint32_t GetWidth() const override { return m_Width; };
		virtual inline uint32_t GetHeight() const override { return m_Height; };

		virtual void Bind(uint32_t slot = 0) const override;

	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
	};

}