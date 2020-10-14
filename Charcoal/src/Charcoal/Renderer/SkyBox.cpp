#include "chpch.h"
#include "SkyBox.h"

namespace Charcoal
{

	SkyBox::SkyBox(const std::string& cubeMapPath)
	{
		m_CubeMap = CubeMap::Create(cubeMapPath);
	}
	
	SkyBox::~SkyBox()
	{
		
	}
	
}
