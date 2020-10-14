#include "chpch.h"
#include "ShaderManager.h"

#include "Charcoal/Core/Core.h"
#include "Charcoal/Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Charcoal
{

	std::unordered_map<std::string, Ref<Shader>> ShaderManager::m_Shaders;
	
	Ref<Shader> ShaderManager::LoadShader(std::string filepath, ShaderLanguage language)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:		CH_CORE_ASSERT(false, "RendererAPI::None not supported!") return nullptr; break;
		case RendererAPI::API::OpenGL:
		{
			Ref<Shader> shader = CreateRef<OpenGLShader>(filepath, language);
			m_Shaders.insert(std::make_pair(shader->GetName(), shader));
			return shader;
			break;
		}
		}
		CH_CORE_ASSERT(false, "Unkown Renderer API!");
		return nullptr;
	}

	Ref<Shader> ShaderManager::GetShader(std::string name)
	{
		return m_Shaders[name];
	}
	
}
