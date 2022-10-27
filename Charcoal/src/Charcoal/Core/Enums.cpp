#include <chpch.h>
#include "Enums.h"

namespace Charcoal
{

	uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::None:       return 0;
			case ShaderDataType::Float:		return 4;
			case ShaderDataType::Float2:	return 4 * 2;
			case ShaderDataType::Float3:	return 4 * 3;
			case ShaderDataType::Float4:	return 4 * 4;
			case ShaderDataType::Mat3:		return 4 * 3 * 3;
			case ShaderDataType::Mat4:		return 4 * 4 * 4;
			case ShaderDataType::Int:		return 4;
			case ShaderDataType::Int2:		return 4 * 2;
			case ShaderDataType::Int3:		return 4 * 3;
			case ShaderDataType::Int4:		return 4 * 4;
			case ShaderDataType::Bool:		return 1;
		}

		CH_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	uint32_t DataTypeSize(DataType type)
	{
		switch(type)
		{
		case DataType::None:      return 0;
		case DataType::Byte:      return 1;
		case DataType::Bool:      return 1;
		case DataType::UInt16:    return 2;
		case DataType::UInt32:    return 4;
		case DataType::Int16:     return 2;
		case DataType::Int32:     return 4;
		case DataType::Float16:   return 2;
		case DataType::Float32:   return 4;
		}
	}
	
}
