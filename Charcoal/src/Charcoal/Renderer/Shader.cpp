#include "chpch.h"
#include "Shader.h"

#include "Charcoal/Core.h"
#include "Charcoal/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Charcoal
{
	Shader* Shader::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		CH_CORE_ASSERT(false, "RendererAPI::None not supported!") return nullptr;
		case RendererAPI::API::OpenGL:	return new OpenGLShader(path);
		}
		CH_CORE_ASSERT(false, "Unkown Renderer API!");
		return nullptr;
	}

	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		CH_CORE_ASSERT(false, "RendererAPI::None not supported!") return nullptr;
		case RendererAPI::API::OpenGL:	return new OpenGLShader(vertexSrc, fragmentSrc);
		}
		CH_CORE_ASSERT(false, "Unkown Renderer API!");
		return nullptr;
	}
}