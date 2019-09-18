#pragma once

#include "chpch.h"

namespace Charcoal
{

	class Shader
	{

	public:
		virtual ~Shader() {}

		static Shader* Create(const std::string& vertexSrc, const std::string& fragmentSrc);

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

	};

}