#include "chpch.h"
#include "OpenGLShader.h"

#include "Charcoal/Core.h"

#include <fstream>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace Charcoal
{
	OpenGLShader::OpenGLShader(const std::string& path)
	{
		std::string shaderSource = ReadFile(path);
		std::unordered_map<GLenum, std::string> shaders = PreProcess(shaderSource);
		Compile(shaders);
	}

	OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc) : m_RendererID()
	{
		std::unordered_map<GLenum, std::string> shaders;
		shaders[GL_VERTEX_SHADER] = vertexSrc;
		shaders[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(shaders);
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaders)
	{
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
		glUniformMatrix4fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, false, glm::value_ptr(matrix));
	}

	void OpenGLShader::SetVec3(const std::string name, const glm::vec3& vec) const
	{
		glUniform3f(glGetUniformLocation(m_RendererID, name.c_str()), vec.x, vec.y, vec.z);
	}

	void OpenGLShader::SetInt(const std::string name, int value) const
	{
		glUniform1i(glGetUniformLocation(m_RendererID, name.c_str()), value);
	}

}