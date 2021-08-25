#pragma once
#include <string>

#if defined CH_PLATFORM_WINDOWS
	#define DEBUG_BREAK() __debugbreak()
#elif defined CH_PLATFORM_LINUX
	#define DEBUG_BREAK() asm("int $3")
#endif

void LoadASCIIGLTF(std::string& filepath);

enum class ShaderDataType // From "Charcoal/Renderer/Buffer.h"
{
	None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
};

enum class TextureType // From "Charcoal/Renderer/Model.h"
{
	None = 0, Albedo, Normal, Metallic, Roughness
};

enum class VertexAttribType // From "Charcoal/Renderer/Mesh.h" Mesh::VertexAttrib::VertexAttribType
{
	None = 0, Position, Normal, Tangent, Bitangent, TexCoord
};

struct CMFFormat
{
	struct Material
	{
		struct Texture
		{
			uint32_t TextureType;

			uint32_t RelativeFilepathSize;
			char* RelativeFilepath;
		};

		uint32_t NameSize;
		char* Name;

		uint32_t ID;

		// Constants / Factors
		float AmbientColour[3];
		float DiffuseColour[3];
		float MetallicFactor;
		float RoughnessFactor;

		// Textures
		uint32_t TextureCount;
		CMFFormat::Material::Texture* Textures;
	};

	struct VertexAttrib
	{
		uint32_t NameSize;
		char* Name;
		uint32_t ShaderDataType;
		uint32_t VertexAttribType;

		uint32_t BufferSize;
		void* Buffer;
	};

	struct Mesh
	{
		uint32_t NameSize;
		char* Name;

		uint32_t MaterialID;

		uint32_t VertexAttribCount;
		CMFFormat::VertexAttrib* VertexAttribs;

		uint32_t IndexBufferSize;
		uint32_t* IndexBuffer;
	};

	const uint32_t HeaderSize = 15;
	const char* Header = "CMFFormatStart";

	uint32_t NameSize;
	char* Name;

	uint32_t MaterialCount;
	CMFFormat::Material* Materials;

	uint32_t MeshCount;
	CMFFormat::Mesh* Meshes;

	const uint32_t FooterSize = 13;
	const char* Footer = "CMFFormatEnd";
};

struct TangentVectors
{
	float Tangent[3];
	float Bitangent[3];
};