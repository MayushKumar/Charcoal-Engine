#pragma once

#include "Charcoal/Core/Core.h"
#include "Charcoal/Core/Enums.h"

namespace Charcoal
{

	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Offset;
		uint32_t Size;
		bool Normalized;

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0) , Normalized(normalized)
		{}

		inline uint32_t GetComponentCount() const
		{
			switch (Type)
			{
				case ShaderDataType::None:      return 0;
				case ShaderDataType::Float:		return 1;
				case ShaderDataType::Float2:	return 2;
				case ShaderDataType::Float3:	return 3;
				case ShaderDataType::Float4:	return 4;
				case ShaderDataType::Mat3:		return 3 * 3;
				case ShaderDataType::Mat4:		return 4 * 4;
				case ShaderDataType::Int:		return 1;
				case ShaderDataType::Int2:		return 2;
				case ShaderDataType::Int3:		return 3;
				case ShaderDataType::Int4:		return 4;
				case ShaderDataType::Bool:		return 1;
			}

			CH_CORE_ASSERT(false, "Unknown ShaderDataType!");
			return 0;
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout() {}
		BufferLayout(const std::initializer_list<BufferElement>& elements) : m_Elements(elements) 
		{
			CalculateOffsetAndStride();
		}

		void AddElement(BufferElement element) { m_Elements.push_back(element); CalculateOffsetAndStride(); }

		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }
		inline uint32_t GetStride() const{ return m_Stride; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

	private:
		void CalculateOffsetAndStride()
		{
			m_Stride = 0;
			uint32_t offset = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}

		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		static Ref<VertexBuffer> Create(uint32_t size);
		static Ref<VertexBuffer> Create(uint32_t size, float* vertices);

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void SetData(float* vertices, uint32_t size) const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual inline const BufferLayout& GetLayout() const = 0;
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		static Ref<IndexBuffer> Create(DataType dataType, uint32_t size, void* indices);

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		uint32_t GetCount() { return m_Size / DataTypeSize(m_DataType); }

	public:
		uint32_t m_Size;
		DataType m_DataType;
	};

}
