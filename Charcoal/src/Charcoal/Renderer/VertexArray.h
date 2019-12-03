#pragma once

#include "Charcoal/Renderer/Buffer.h"
#include "Charcoal/Core/Core.h"

namespace Charcoal
{

	class VertexArray
	{
	public:
		virtual ~VertexArray() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const Ref<VertexBuffer> buffer) = 0;
		virtual void SetIndexBuffer(const Ref<IndexBuffer> buffer) = 0;

		virtual IndexBuffer* GetIndexBuffer() const = 0;

		static VertexArray* Create();
	};

}