#pragma once

namespace Charcoal {

	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	enum class DataType
	{
		None = 0, Byte, Bool, UInt16, UInt32, Int16, Int32, Float16, Float32
	};

	uint32_t ShaderDataTypeSize(ShaderDataType type);
	uint32_t DataTypeSize(DataType type);
}
