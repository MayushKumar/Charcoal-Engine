#pragma once

#include "chpch.h"

#include <Charcoal/Core.h>

#include <glm/glm.hpp>

namespace Charcoal
{

	class Shader
	{

	public:
		virtual ~Shader() {}

		static Ref<Shader> Create(const std::string& filepath);
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual std::string GetName() const = 0;

		virtual void SetMat4(const std::string name, const glm::mat4& matrix) const = 0;
		virtual void SetVec3(const std::string name, const glm::vec3& vec) const = 0;
		virtual void SetInt(const std::string name, int value) const = 0;

	};

	class ShaderLibrary
	{
	public:
		void Add(const std::string& name, const Ref<Shader>& shader);
		void Add(const Ref<Shader>& shader);
		Ref<Shader> Load(const std::string& filepath);
		Ref<Shader> Load(const std::string& name, std::string& filepath);
		Ref<Shader> Get(const std::string name);
		inline bool Exists(const std::string& name) const { return m_Shaders.find(name) != m_Shaders.end(); }

	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};

}