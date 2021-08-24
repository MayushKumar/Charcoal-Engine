#pragma once

#include "Charcoal/Core/Core.h"
#include "Charcoal/Renderer/Texture.h"
#include "Charcoal/Renderer/VertexArray.h"
#include "Charcoal/Renderer/Framebuffer.h"

namespace Charcoal
{

	class SkyBox
	{
	public:
		SkyBox(const std::string& equiMapPath);
		~SkyBox();

	public:
		Ref<CubeMap> m_CubeMap;
		Ref<CubeMap> m_IrradianceMap;
		Ref<CubeMap> m_RadianceMap;
		float m_LuminanceFactor = 1.0f;

		static Ref<Texture2D> s_SpecularLUT;
		
	private:
		void GenerateIrradianceMap();
		void PrefilterRadianceMap();
	};
	
}
