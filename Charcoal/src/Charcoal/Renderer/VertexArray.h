#pragma once

#include <memory>
#include "Charcoal/Renderer/Buffer.h"

namespace Charcoal
{

	class VertexArray
	{
	public:
		virtual ~VertexArray() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer> buffer) = 0;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer> buffer) = 0;

		virtual IndexBuffer* GetIndexBuffer() const = 0;

		static VertexArray* Create();
	};

}