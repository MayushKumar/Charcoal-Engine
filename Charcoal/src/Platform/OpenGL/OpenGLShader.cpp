#include "chpch.h"
#include "OpenGLShader.h"

#include "Charcoal/Core/Core.h"
#include "Charcoal/Core/Utilities.h"

#include <fstream>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#define CH_DEBUG_OPENGL_SHADERS 1

namespace Charcoal
{
	OpenGLShader::OpenGLShader(const std::string& filepath, ShaderLanguage language) : m_Filepath(filepath), m_Language(language)
	{
		CH_PROFILE_FUNCTION();
		m_RendererID = -1;
		ReadAndCompile();
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) : m_RendererID(), m_Name(name)
	{
		// CH_PROFILE_FUNCTION();

		// std::unordered_map<GLenum, std::string> shaders;
		// shaders[GL_VERTEX_SHADER] = vertexSrc;
		// shaders[GL_FRAGMENT_SHADER] = fragmentSrc;
		// CompileShaderSources(shaders);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		#if CH_DEBUG_OPENGL_SHADERS 
			if (m_CompilationSuccess)
		#endif
				glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::ReadAndCompile()
	{
		CH_PROFILE_FUNCTION();
		CH_CORE_ASSERT(!m_Filepath.empty(), "ReadAndCompile cannot be called on a Shader with no asssociated filepath");

		m_CompilationSuccess = true;

		m_LocationCache.clear();
		if (m_RendererID != -1)
			glDeleteProgram(m_RendererID);
		
		switch (m_Language)
		{
		case ShaderLanguage::SPIRV:
		{
			CompileSPIRVShaderSources(FileUtils::ReadFileBinary(m_Filepath));
			break;
		}	
		case ShaderLanguage::GLSL:
		{
 			std::string shaderSource = ReadFile(m_Filepath);
			std::unordered_map<GLenum, std::string> shaders = PreProcess(shaderSource);
			CompileGLSLShaderSources(shaders);
			break;
		}	
		case ShaderLanguage::None:
		{
			CH_CORE_ERROR("Shader Language not supported for: {0}", m_Filepath);
			break;
		}
		}

		size_t slash = m_Filepath.find_last_of("/\\");
		slash = slash == std::string::npos ? 0 : slash;
		size_t dot = m_Filepath.rfind('.');
		dot = dot == std::string::npos ? m_Filepath.length() : dot;
		m_Name = m_Filepath.substr(slash + 1, dot - slash - 1);
	}

	void OpenGLShader::CompileGLSLShaderSources(const std::unordered_map<GLenum, std::string>& shaders)
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
				#if CH_DEBUG_OPENGL_SHADERS
					m_CompilationSuccess = false;
				#else
					CH_CORE_ASSERT(false, "Shader compilation error");
				#endif
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
			#if CH_DEBUG_OPENGL_SHADERS
				m_CompilationSuccess = false;
			#else
				CH_CORE_ASSERT(false, "Shader linking error")
			#endif
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

	void OpenGLShader::CompileSPIRVShaderSources(const std::vector<char>& shaderBinary)
	{
		CH_PROFILE_FUNCTION();

		GLuint program = glCreateProgram();

		//VERTEX SHADER
		GLuint vertID = glCreateShader(GL_VERTEX_SHADER);
		glShaderBinary(1, &vertID, GL_SHADER_BINARY_FORMAT_SPIR_V, shaderBinary.data(), shaderBinary.size());

		glSpecializeShader(vertID, "main", 0, nullptr, nullptr);

		GLint vertIsCompiled = 0;
		glGetShaderiv(vertID, GL_COMPILE_STATUS, &vertIsCompiled);
		if (vertIsCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertID, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertID, maxLength, &maxLength, &infoLog[0]);

			glDeleteShader(vertID);

			CH_CORE_ERROR(infoLog.data());
#if CH_DEBUG_OPENGL_SHADERS
			m_CompilationSuccess = false;
#else
			CH_CORE_ASSERT(false, "Shader compilation error");
#endif
			return;
		}
		glAttachShader(program, vertID);

		//FRAGMENT SHADER
		GLuint fragID = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderBinary(1, &fragID, GL_SHADER_BINARY_FORMAT_SPIR_V, shaderBinary.data(), shaderBinary.size());

		glSpecializeShader(fragID, "main", 0, nullptr, nullptr);

		GLint fragIsCompiled = 0;
		glGetShaderiv(fragID, GL_COMPILE_STATUS, &fragIsCompiled);
		if (fragIsCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragID, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragID, maxLength, &maxLength, &infoLog[0]);

			glDeleteShader(fragID);

			CH_CORE_ERROR(infoLog.data());
#if CH_DEBUG_OPENGL_SHADERS
			m_CompilationSuccess = false;
#else
			CH_CORE_ASSERT(false, "Shader compilation error");
#endif
			return;
		}

		glAttachShader(program, fragID);
		
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
			glDeleteShader(vertID);
			glDeleteShader(fragID);
			// Use the infoLog as you see fit.
			CH_CORE_ERROR(infoLog.data());
			#if CH_DEBUG_OPENGL_SHADERS
				m_CompilationSuccess = false;
			#else
				CH_CORE_ASSERT(false, "Shader linking error")
			#endif
				return;
		}

		// Always detach shaders after a successful link.
	    glDetachShader(program, vertID);
		glDetachShader(program, fragID);
		glDeleteShader(vertID);
		glDeleteShader(fragID);

		m_RendererID = program;

	}

	std::string OpenGLShader::ReadFile(const std::string& path)
	{
		CH_PROFILE_FUNCTION();

		std::string result;
		std::ifstream file(path, std::ios::in | std::ios::binary);

		if (!file)
		{
			CH_CORE_ERROR("Could not open file: {0}", path);
			m_CompilationSuccess = false;
		}

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

	void OpenGLShader::SetMat3(const std::string name, const glm::mat3& matrix) const
	{
		#if CH_DEBUG_OPENGL_SHADERS 
				if (m_CompilationSuccess)
		#endif
			glUniformMatrix3fv(GetUniformLocation(name), 1, false, glm::value_ptr(matrix));
	}

	void OpenGLShader::SetMat4(const std::string name, const glm::mat4& matrix) const
	{
		#if CH_DEBUG_OPENGL_SHADERS 
			if (m_CompilationSuccess)
		#endif
			glUniformMatrix4fv(GetUniformLocation(name), 1, false, glm::value_ptr(matrix));
	}

	void OpenGLShader::SetVec3(const std::string name, const glm::vec3& vec) const
	{
		 #if CH_DEBUG_OPENGL_SHADERS 
			if (m_CompilationSuccess)
		#endif
			glUniform3f(GetUniformLocation(name), vec.x, vec.y, vec.z);
	}

	void OpenGLShader::SetVec4(const std::string name, const glm::vec4& vec) const
	{
		#if CH_DEBUG_OPENGL_SHADERS 
			if (m_CompilationSuccess)
		#endif
			glUniform4f(GetUniformLocation(name), vec.x, vec.y, vec.z, vec.w);
	}

	void OpenGLShader::SetFloat(const std::string name, float value) const
	{
		#if CH_DEBUG_OPENGL_SHADERS 
			if (m_CompilationSuccess)
		#endif
			glUniform1f(GetUniformLocation(name), value);
	}

	void OpenGLShader::SetInt(const std::string name, int value) const
	{
		#if CH_DEBUG_OPENGL_SHADERS 
			if (m_CompilationSuccess)
		#endif
			glUniform1i(GetUniformLocation(name), value);
	}

	void OpenGLShader::SetIntArray(const std::string name, uint32_t count, int32_t* value) const
	{
		#if CH_DEBUG_OPENGL_SHADERS 
			if (m_CompilationSuccess)
		#endif
			glUniform1iv(GetUniformLocation(name), count, value);
	}

	void OpenGLShader::SetUIntArray(const std::string name, uint32_t count, uint32_t* value) const
	{
		#if CH_DEBUG_OPENGL_SHADERS 
			if (m_CompilationSuccess)
		#endif
			glUniform1uiv(GetUniformLocation(name), count, value);
	}

}
