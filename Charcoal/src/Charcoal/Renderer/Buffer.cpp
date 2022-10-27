#include "chpch.h"
#include "Buffer.h"

#include "Charcoal/Renderer/RendererAPI.h"
#include "Charcoal/Core/Core.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Charcoal
{

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:		CH_CORE_ASSERT(false, "RendererAPI::None not supported!") return nullptr;
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLVertexBuffer>(size);
		}
		CH_CORE_ASSERT(false, "Unkown Renderer API!");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size, float* vertices)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:		CH_CORE_ASSERT(false, "RendererAPI::None not supported!") return nullptr;
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLVertexBuffer>(size, vertices);
		}
		CH_CORE_ASSERT(false, "Unkown Renderer API!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(DataType dataType, uint32_t size, void* indices)
	{
		Ref<IndexBuffer> ref;
		switch (RendererAPI::GetAPI())
		{
		case	RendererAPI::API::None:		CH_CORE_ASSERT(false, "RendererAPI::None not supported!") return nullptr;
		case	RendererAPI::API::OpenGL:	ref = CreateRef<OpenGLIndexBuffer>(size, indices); break;
		default: CH_CORE_ASSERT(false, "Unkown Renderer API!"); return nullptr;
		}
		ref->m_DataType = dataType;
		ref->m_Size = size;
		return ref;
	}

}
