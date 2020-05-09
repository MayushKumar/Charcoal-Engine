#pragma once

#include "Charcoal/Renderer/Texture.h"

namespace Charcoal
{

	enum class TextureType
	{
		None = 0, Albedo, Normal, Metallic, Roughness
	};

	class Material
	{
	public:
		std::string m_Name;

		std::vector<Ref<Texture2D>> m_Textures;
		std::vector<TextureType> m_TextureTypes;

		float m_AmbientColour[3];
		float m_DiffuseColour[3];
		float m_MetallicFactor;
		float m_RoughnessFactor;
	};

}