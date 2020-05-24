#include "chpch.h"
#include "OpenGLShader.h"

#include "Charcoal/Core/Core.h"

#include <fstream>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace Charcoal
{
	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		CH_PROFILE_FUNCTION();

		std::string shaderSource = ReadFile(filepath);
		std::unordered_map<GLenum, std::string> shaders = PreProcess(shaderSource);
		Compile(shaders);
		size_t slash = filepath.find_last_of("/\\");
		slash = slash == std::string::npos ? 0 : slash;
		size_t dot = filepath.rfind('.');
		dot = dot == std::string::npos ? filepath.length() : dot;
		m_Name = filepath.substr(slash + 1, dot - slash - 1);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) : m_RendererID(), m_Name(name)
	{
		CH_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> shaders;
		shaders[GL_VERTEX_SHADER] = vertexSrc;
		shaders[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(shaders);
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaders)
	{
		CH_PROFILE_FUNCTION();

		GLuint program = glCreateProgram();
		CH_CORE_ASSERT((shaders.size() <= 2), "Size of shader sources should not be more than 2!");
		std::array<GLuint, 2> shaderIDs;
		int shaderIDIndex = 0;
		for (auto& shader : shaders)
		{
			GLuint shaderID = glCreateShader(shader.first);
			const GLchar* source = (const GLchar*)shader.second.c_str();
			glShaderSource(shaderID, 1, &source, 0);

			glCompileShader(shaderID);

			GLint isCompiled = 0;
			glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shaderID, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shaderID);

				CH_CORE_ERROR(infoLog.data());
				CH_CORE_ASSERT(false, "Shader compilation error");
				return;
			}

			glAttachShader(program, shaderID);
			shaderIDs[shaderIDIndex++] = shaderID;
		}
		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			for (GLuint shaderID : shaderIDs)
			{
				glDeleteShader(shaderID);
			}
			// Use the infoLog as you see fit.
			CH_CORE_ERROR(infoLog.data());
			CH_CORE_ASSERT(false, "Shader linking error")
				return;
		}

		// Always detach shaders after a successful link.
		for (GLuint shaderID : shaderIDs) 
		{
			glDetachShader(program, shaderID);
			glDeleteShader(shaderID);
		}

		m_RendererID = program;
	}

	std::string OpenGLShader::ReadFile(const std::string& path)
	{
		CH_PROFILE_FUNCTION();

		std::string result;
		std::ifstream file(path, std::ios::in | std::ios::binary);

		if (!file) CH_CORE_ERROR("Could not open file: {0}", path);

		file.seekg(0, std::ios::end);
		result.resize(file.tellg());
		file.seekg(0, std::ios::beg);
		file.read(&result[0], result.size());
		file.close();
		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string shaderSource)
	{
		CH_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> map = std::unordered_map<GLenum, std::string>();
		size_t index = 0;
		size_t carriageReturn = 0;
		while (index != std::string::npos)
		{
			GLenum type;
			size_t tokenPos = shaderSource.find("#type ", index);
			index = tokenPos == std::string::npos ? std::string::npos : (tokenPos + 6);
			if (index != std::string::npos) {
				carriageReturn = shaderSource.find_first_of("\r\n", index);
				std::string typeStr = shaderSource.substr(index, carriageReturn - index);
				std::string section = shaderSource.substr(carriageReturn, shaderSource.find("#type ", carriageReturn) - carriageReturn);
				if (typeStr == "vertex") type = GL_VERTEX_SHADER;
				else if (typeStr == "fragment" || typeStr == "pixel") type = GL_FRAGMENT_SHADER;

				map.insert({ type, section });
			}
		}
		return map;
	}

	uint32_t OpenGLShader::GetUniformLocation(const std::string& name) const
	{
		CH_PROFILE_FUNCTION();

		auto iterator = m_LocationCache.find(name);
		if (iterator == m_LocationCache.end())
		{
			uint32_t index = glGetUniformLocation(m_RendererID, name.c_str());
			m_LocationCache.insert({name, index});
			return index;
		}
		else
		{
			return m_LocationCache.find(name)->second;
		}
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::SetMat4(const std::string name, const glm::mat4& matrix) const
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, false, glm::value_ptr(matrix));
	}

	void OpenGLShader::SetVec3(const std::string name, const glm::vec3& vec) const
	{
		glUniform3f(GetUniformLocation(name), vec.x, vec.y, vec.z);
	}

	void OpenGLShader::SetVec4(const std::string name, const glm::vec4& vec) const
	{
		glUniform4f(GetUniformLocation(name), vec.x, vec.y, vec.z, vec.w);
	}

	void OpenGLShader::SetFloat(const std::string name, float value) const
	{
		glUniform1f(GetUniformLocation(name), value);
	}

	void OpenGLShader::SetInt(const std::string name, int value) const
	{
		glUniform1i(GetUniformLocation(name), value);
	}

	void OpenGLShader::SetIntArray(const std::string name, uint32_t count, int32_t* value) const
	{
		glUniform1iv(GetUniformLocation(name), count, value);
	}

	void OpenGLShader::SetUIntArray(const std::string name, uint32_t count, uint32_t* value) const
	{
		glUniform1uiv(GetUniformLocation(name), count, value);
	}

}
