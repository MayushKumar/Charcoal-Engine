#include "chpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Charcoal
{

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case Charcoal::ShaderDataType::None:	return GL_NONE;
		case Charcoal::ShaderDataType::Float:	return GL_FLOAT;
		case Charcoal::ShaderDataType::Float2:	return GL_FLOAT;
		case Charcoal::ShaderDataType::Float3:	return GL_FLOAT;
		case Charcoal::ShaderDataType::Float4:	return GL_FLOAT;
		case Charcoal::ShaderDataType::Mat3:	return GL_FLOAT;
		case Charcoal::ShaderDataType::Mat4:	return GL_FLOAT;
		case Charcoal::ShaderDataType::Int:		return GL_INT;
		case Charcoal::ShaderDataType::Int2:	return GL_INT;
		case Charcoal::ShaderDataType::Int3:	return GL_INT;
		case Charcoal::ShaderDataType::Int4:	return GL_INT;
		case Charcoal::ShaderDataType::Bool:	return GL_BOOL;
		}
		CH_CORE_ASSERT(false, "Unkown ShaderDataType!");
		return GL_NONE;
	}

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

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer> buffer)
	{
		CH_PROFILE_FUNCTION();

		CH_CORE_ASSERT(buffer->GetLayout().GetElements().size(), "VertexBuffer Layout has not been set");

		glBindVertexArray(m_RendererID);
		buffer->Bind();

		uint32_t index = 0;
		const auto& layout = buffer->GetLayout();
		for (const auto& element : layout)
		{
			glVertexAttribPointer(index, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.Type), element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)element.Offset);
			glEnableVertexAttribArray(index);
			index++;
		}
		m_VertexBuffers.push_back(buffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer> buffer)
	{
		CH_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
		buffer->Bind();
		m_IndexBuffer = buffer;
	}

}