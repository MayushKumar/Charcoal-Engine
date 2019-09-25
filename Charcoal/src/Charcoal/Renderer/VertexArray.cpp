#include "chpch.h"
#include "VertexArray.h"

#include "Charcoal/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Charcoal
{

	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::None:		CH_CORE_ASSERT(false, "RendererAPI::None not supported!") return nullptr;
			case RendererAPI::OpenGL:	return new OpenGLVertexArray();
		}
		CH_CORE_ASSERT(false, "Unkown Renderer API!");
		return nullptr;
	}

}