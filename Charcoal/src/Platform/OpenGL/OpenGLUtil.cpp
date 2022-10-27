#include <chpch.h>
#include "OpenGLUtil.h"

namespace Charcoal {


	 GLenum DataTypeToOpenGLDataType(DataType type)
	{
		switch (type) {
		case DataType::Bool: return GL_BOOL;
		case DataType::Byte: return GL_BYTE;
		case DataType::UInt16: return GL_UNSIGNED_SHORT;
		case DataType::UInt32: return GL_UNSIGNED_INT;
		case DataType::Int16: return GL_SHORT;
		case DataType::Int32: return GL_INT;
		case DataType::Float32: return GL_FLOAT;
		}
	}

	GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
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
}
