#pragma once

#include "chpch.h"

#include "Charcoal/Core/Core.h"

#include <glm/glm.hpp>

namespace Charcoal
{

	class Shader
	{

	public:
		virtual ~Shader() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void ReadAndCompile() = 0; 
		
		virtual std::string GetName() const = 0;

		virtual void SetMat3(const std::string name, const glm::mat3& matrix) const = 0;
		virtual void SetMat4(const std::string name, const glm::mat4& matrix) const = 0;
		virtual void SetVec3(const std::string name, const glm::vec3& vec) const = 0;
		virtual void SetVec4(const std::string name, const glm::vec4& vec) const = 0;
		virtual void SetFloat(const std::string name, float value) const = 0;
		virtual void SetInt(const std::string name, int value) const = 0;
		virtual void SetIntArray(const std::string name, uint32_t count, int32_t* value) const = 0;
		virtual void SetUIntArray(const std::string name, uint32_t count, uint32_t* value) const = 0;

	};

}
