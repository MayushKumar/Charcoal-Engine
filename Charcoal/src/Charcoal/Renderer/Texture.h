#pragma once

#include "Charcoal/Core/Core.h"
#include <glm/glm.hpp>

namespace Charcoal
{

	class Texture2D
	{
	public:
		static Ref<Texture2D> Create(const std::string& path);
		static Ref<Texture2D> Create(uint32_t width, uint32_t height, void* data);

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
	};

	class CubeMap
	{
	public:
		static Ref<CubeMap> Create(const std::string& path);

		virtual void Bind(uint32_t slot = 0) const = 0;
	};

}
