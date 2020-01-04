#pragma once
#include <unordered_map>

#include "Charcoal/Renderer/Shader.h"

#include <glm/glm.hpp>
#include <glad/glad.h>

namespace Charcoal
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual std::string GetName() const override { return m_Name; };

		virtual void SetMat4(const std::string name, const glm::mat4& matrix) const override;
		virtual void SetVec3(const std::string name, const glm::vec3& vec) const override;
		virtual void SetVec4(const std::string name, const glm::vec4& vec) const override;
		virtual void SetInt(const std::string name, int value) const override;

	private:
		void Compile(const std::unordered_map<GLenum, std::string>& shaders);
		std::string ReadFile(const std::string& path);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string shaderSource);
		uint32_t GetUniformLocation(const std::string& name) const;

	private:
		mutable std::unordered_map<std::string, uint32_t> m_LocationCache;
		uint32_t m_RendererID;
		std::string m_Name;
	};
}