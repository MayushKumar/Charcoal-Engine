#include "chpch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Charcoal/Core/Core.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Charcoal
{

	VertexBuffer* VertexBuffer::Create(uint32_t size, float* vertices)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		CH_CORE_ASSERT(false, "RendererAPI::None not supported!") return nullptr;
		case RendererAPI::API::OpenGL:	return new OpenGLVertexBuffer(size, vertices);
		}
		CH_CORE_ASSERT(false, "Unkown Renderer API!");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t size, uint32_t* indices)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		CH_CORE_ASSERT(false, "RendererAPI::None not supported!") return nullptr;
		case  RendererAPI::API::OpenGL:	return new OpenGLIndexBuffer(size, indices);
		}
		CH_CORE_ASSERT(false, "Unkown Renderer API!");
		return nullptr;
	}

}