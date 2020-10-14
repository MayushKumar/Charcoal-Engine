#pragma once

#include "Charcoal/Core/Core.h"
#include "Charcoal/Renderer/Texture.h"

namespace Charcoal
{

	class SkyBox
	{
	public:
		SkyBox(const std::string& cubeMapPath);
		~SkyBox();

		inline Ref<CubeMap> GetCubeMap() const { return m_CubeMap; }
		
	private:
		Ref<CubeMap> m_CubeMap;
	};
	
}
