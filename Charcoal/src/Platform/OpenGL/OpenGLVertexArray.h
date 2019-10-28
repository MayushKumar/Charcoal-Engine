#pragma once

#include "Charcoal/Renderer/VertexArray.h"

namespace Charcoal
{

	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const Ref<VertexBuffer> buffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer> buffer) override;

		virtual IndexBuffer* GetIndexBuffer() const override { return m_IndexBuffer.get(); }
	private:
		uint32_t m_RendererID = 0;
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;
	};

}