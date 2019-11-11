#include <chpch.h>
#include "Texture.h"

#include "Charcoal/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Charcoal
{

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: CH_CORE_ERROR("None is not a supported API type!");
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLTexture2D>(path);
		}
		return Ref<Texture2D>();
	}

}