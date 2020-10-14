#include "chpch.h"
#include "VertexArray.h"

#include "Charcoal/Renderer/RendererAPI.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Charcoal
{

	Ref<VertexArray> VertexArray::Create()
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:	CH_CORE_ASSERT(false, "RendererAPI::None not supported!") return nullptr;
			case RendererAPI::API::OpenGL:	return std::make_shared<OpenGLVertexArray>();
		}
		CH_CORE_ASSERT(false, "Unkown Renderer API!");
		return nullptr;
	}

}