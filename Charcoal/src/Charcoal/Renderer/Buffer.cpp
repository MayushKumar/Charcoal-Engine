#include "chpch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Charcoal/Core/Core.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Charcoal
{

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size, float* vertices)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		CH_CORE_ASSERT(false, "RendererAPI::None not supported!") return nullptr;
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLVertexBuffer>(size, vertices);
		}
		CH_CORE_ASSERT(false, "Unkown Renderer API!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t size, uint32_t* indices)
	{
		switch (Renderer::GetAPI())
		{
		case	RendererAPI::API::None:		CH_CORE_ASSERT(false, "RendererAPI::None not supported!") return nullptr;
		case	RendererAPI::API::OpenGL:	return CreateRef<OpenGLIndexBuffer>(size, indices);
		}
		CH_CORE_ASSERT(false, "Unkown Renderer API!");
		return nullptr;
	}

}