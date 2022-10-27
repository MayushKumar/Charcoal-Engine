#pragma once

#include "Charcoal/Renderer/Buffer.h"

namespace Charcoal
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(uint32_t size, float* vertices);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(float* vertices, uint32_t size) const override;
		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
		virtual inline const BufferLayout& GetLayout() const override { return m_Layout; }
	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t size, void* indices);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;
		// inline virtual uint32_t GetCount() const override { return m_Size / sizeof(uint32_t); }

	private:
		uint32_t m_RendererID;
	};
}
