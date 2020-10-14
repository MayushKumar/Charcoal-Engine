#pragma once
#include "Charcoal/Renderer/Shader.h"

#include "Charcoal/Renderer/ShaderManager.h"

#include <unordered_map>

#include <glm/glm.hpp>
#include <glad/glad.h>

namespace Charcoal
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath, ShaderLanguage language);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void ReadAndCompile() override;

		virtual std::string GetName() const override { return m_Name; };

		virtual void SetMat3(const std::string name, const glm::mat3& matrix) const override;
		virtual void SetMat4(const std::string name, const glm::mat4& matrix) const override;
		virtual void SetVec3(const std::string name, const glm::vec3& vec) const override;
		virtual void SetVec4(const std::string name, const glm::vec4& vec) const override;
		virtual void SetFloat(const std::string name, float value) const override;
		virtual void SetInt(const std::string name, int value) const override;
		virtual void SetIntArray(const std::string name, uint32_t count, int32_t* value) const override;
		virtual void SetUIntArray(const std::string name, uint32_t count, uint32_t* value) const override;

	private:
		void CompileGLSLShaderSources(const std::unordered_map<GLenum, std::string>& shaders);
		void CompileSPIRVShaderSources(const std::vector<char>& shaderBinary);
		std::string ReadFile(const std::string& path);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string shaderSource);
		uint32_t GetUniformLocation(const std::string& name) const;

	private:
		mutable std::unordered_map<std::string, uint32_t> m_LocationCache;
		uint32_t m_RendererID;
		std::string m_Name;
		std::string m_Filepath;
		ShaderLanguage m_Language;
		bool m_CompilationSuccess;
	};
}
