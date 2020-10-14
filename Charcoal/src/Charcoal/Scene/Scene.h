#pragma once

#include "Charcoal/Core/Core.h"
#include "Charcoal/Renderer/PerspectiveCamera.h"
#include "Charcoal/Renderer/SkyBox.h"
#include "Charcoal/Renderer/Light.h"

namespace Charcoal
{

	class Scene3D
	{
	public:
		Scene3D() = default;
		~Scene3D() {}
		
		void SetSkyBox(Ref<SkyBox> skyBox) { m_SkyBox = skyBox; m_SkyBoxEnabled = true; }
		void SetSkyBoxEnabled(bool enabled) { m_SkyBoxEnabled = enabled; }
		bool IsSkyBoxEnabled() { return m_SkyBoxEnabled; }
		
		inline Ref<SkyBox> GetSkyBox() const { return m_SkyBox; }

	public:
		PerspectiveCamera m_Camera;
		std::vector<PointLight> m_PointLights;
		
	private:
		Ref<SkyBox> m_SkyBox;
		bool m_SkyBoxEnabled;
	};
	
}
