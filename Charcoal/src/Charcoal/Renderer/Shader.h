#pragma once

#include "chpch.h"

#include <glm/glm.hpp>

namespace Charcoal
{

	class Shader
	{

	public:
		virtual ~Shader() {}

		static Shader* Create(const std::string& path);
		static Shader* Create(const std::string& vertexSrc, const std::string& fragmentSrc);

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetMat4(const std::string name, const glm::mat4& matrix) const = 0;
		virtual void SetVec3(const std::string name, const glm::vec3& vec) const = 0;
		virtual void SetInt(const std::string name, int value) const = 0;

	};

}