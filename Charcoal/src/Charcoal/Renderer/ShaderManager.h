#pragma once

#include "Charcoal/Renderer/Shader.h"

#include <unordered_map>

namespace Charcoal
{
	
	enum class ShaderLanguage
	{
		None = 0, SPIRV, GLSL
	};
	
	class ShaderManager
	{
	public:
		static Ref<Shader> LoadShader(std::string filepath, ShaderLanguage language);
		static Ref<Shader> GetShader(std::string name);
		
	private:
		static std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};
	
}
