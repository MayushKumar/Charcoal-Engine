#include "chpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>
#include "OpenGLUtil.h"

namespace Charcoal
{


	OpenGLVertexArray::OpenGLVertexArray()
	{
		CH_PROFILE_FUNCTION();

		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		CH_PROFILE_FUNCTION();

		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		CH_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		CH_PROFILE_FUNCTION();

		glBindVertexArray(0);
	}

	// Call only if the VertexArray is bound.
	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer> buffer, uint32_t attribOffset)
	{
		CH_PROFILE_FUNCTION();

		CH_CORE_ASSERT(buffer->GetLayout().GetElements().size(), "VertexBuffer Layout has not been set");

		glBindVertexArray(m_RendererID);
		buffer->Bind();

		uint32_t index = attribOffset;
		const auto& layout = buffer->GetLayout();
		for (const auto& element : layout)
		{
			glVertexAttribPointer(index, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.Type), element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)element.Offset);
			glEnableVertexAttribArray(index);
			index++;
		}
		m_VertexBuffers.push_back(buffer);
	}

	// Call only if the VertexArray is bound.
	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer> buffer)
	{
		CH_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
		buffer->Bind();
		m_IndexBuffer = buffer;
	}

}
