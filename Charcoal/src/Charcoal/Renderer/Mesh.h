#pragma once

#include <string>
#include <vector>

#include "Charcoal/Renderer/Buffer.h"

namespace Charcoal
{

	class Mesh
	{
	public:
		struct VertexAttrib
		{
			enum class VertexAttribType
			{
				None = 0, Position, Normal, TexCoord
			};

			std::string Name;
			VertexAttribType AttribType;
			ShaderDataType DataType;
			std::vector<char> Buffer;
		};

	public:
		std::string Name;

		uint32_t m_MaterialIndex;

		std::vector<VertexAttrib> m_VertexAttribs;
		std::vector<uint32_t> m_IndexBufferData;
	};

}