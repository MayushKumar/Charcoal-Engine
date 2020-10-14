#include "chpch.h"
#include "Material.h"

namespace Charcoal
{

	//PBR Material
	PBRMaterial::PBRMaterial()
	{
		
	}

	PBRMaterial::~PBRMaterial()
	{
		
	}

	void PBRMaterial::Bind() const
	{
		m_Shader->Bind();
	}

	void PBRMaterial::Unbind() const
	{
		m_Shader->Unbind();
	}
	
}
