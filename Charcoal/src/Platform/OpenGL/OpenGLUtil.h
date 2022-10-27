#pragma once

#include <glad/glad.h>

#include "Charcoal/Core/Enums.h"

namespace Charcoal {

	GLenum DataTypeToOpenGLDataType(DataType type);
	GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type);
}
