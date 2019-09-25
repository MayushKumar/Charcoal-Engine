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

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer> buffer) override;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer> buffer) override;

		virtual IndexBuffer* GetIndexBuffer() const override { return m_IndexBuffer.get(); }
	private:
		uint32_t m_RendererID = 0;
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
	};

}