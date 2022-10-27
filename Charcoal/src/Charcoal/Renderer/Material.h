#pragma once

#include "Charcoal/Renderer/Shader.h"
#include "Charcoal/Renderer/Texture.h"

namespace Charcoal
{


	class Material
	{
	protected:
		Material() {}
		
	public:
		enum class TextureType
		{
			None = 0, Albedo, Normal, Metallic, Roughness, Metallic_Roughness
		};
		virtual ~Material() {}
		
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
	};

	class PBRMaterial : public Material
	{
	public:
		PBRMaterial();
		~PBRMaterial();
		
		virtual void Bind() const override;
		virtual void Unbind() const override;

	public:
		std::string m_Name;
		Ref<Shader> m_Shader;
		
		Ref<Texture2D> m_AlbedoMap;
		Ref<Texture2D> m_NormalMap;
		// Ref<Texture2D> m_MetallicMap;
		// Ref<Texture2D> m_RoughnessMap;
		Ref<Texture2D> m_MetallicRoughnessMap;

		// float m_AmbientColour[3];
		// float m_DiffuseColour[3];
		float m_BaseColour[4];
		float m_MetallicFactor;
		float m_RoughnessFactor;
	};

}
