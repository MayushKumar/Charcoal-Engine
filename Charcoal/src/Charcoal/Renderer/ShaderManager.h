#pragma once

#include "Charcoal/Renderer/Shader.h"

#include <map>

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
		
	public:
		static std::map<std::string, Ref<Shader>> shader_map;
	};
	
}
