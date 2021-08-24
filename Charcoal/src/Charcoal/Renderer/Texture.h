#pragma once

#include "Charcoal/Core/Core.h"
#include "Charcoal/Renderer/Shader.h"

#include <glm/glm.hpp>

namespace Charcoal
{

	enum class TextureType : uint32_t
	{
		None = 0,
		Texture2D,
		CubeMap,
		CubeMapFace
	};
   
	enum class TextureFormat : uint32_t
	{
		None = 0, R, RG, RGB, RGBA, Depth, Stencil
	};

	enum class TextureDataType : uint32_t
	{
		None = 0, UByte, Float
	};

	enum class TextureInternalFormat : uint32_t
	{
		None = 0,
		R, RG, RGB, RGBA,
		R16F, RG16F, RGB16F, RGBA16F,
		R32F, RG32F, RGB32F, RGBA32F,
		Depth16, Depth32, Depth32F,
		Stencil
	};

	// TODO: Allow explicitly specifying magnification filters
	class Texture2D
	{
	public:
		struct Texture2DSpec
		{
			TextureInternalFormat InternalFormat;
			TextureFormat Format;
			TextureDataType DataType;
			uint32_t MipCount = 1;
		};

	public:
		static Ref<Texture2D> Create(Texture2DSpec spec, uint32_t width, uint32_t height,void* data);
		// static Ref<Texture2D> Create(const std::string& path);
		static Ref<Texture2D> Create(Texture2DSpec spec, const std::string& path);
		static Ref<Texture2D> Create(uint32_t width, uint32_t height, void* data);

		virtual void Bind(uint32_t slot = 0) const = 0;

		inline uint32_t GetWidth() const { return m_Width; };
		inline uint32_t GetHeight() const { return m_Height; };

		virtual uint32_t GetRendererID() const = 0;

		virtual uint32_t GetDataSize() const = 0;
		virtual void GetData(void* location) const = 0;

		TextureFormat GetFormat() { return m_Spec.Format; }
		TextureInternalFormat GetInternalFormat() { return m_Spec.InternalFormat; }

	protected:
		Texture2DSpec m_Spec;
		uint32_t m_DataSize;
		uint32_t m_Width, m_Height;
	};

	class CubeMap
	{
	public:

		enum class CubeMapFace
		{
			PosX = 0, NegX, PosY, NegY, PosZ, NegZ
		};

		static Ref<CubeMap> Create(uint32_t size, TextureInternalFormat internalFormat, uint32_t mipCount = 1);
		static Ref<CubeMap> Create(const std::string& path);

		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual uint32_t GetRendererID() const = 0;

		uint32_t GetSize() { return m_Size; }

	protected:
		uint32_t m_Size;
	};

}
