#pragma once

#include "Charcoal/Renderer/Buffer.h"
#include "Charcoal/Renderer/VertexArray.h"
#include "Charcoal/Renderer/Texture.h"
#include "Charcoal/Renderer/Material.h"
#include "Charcoal/Renderer/Mesh.h"

#include <glm/glm.hpp>

namespace Charcoal
{

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
			// float AmbientColour[3];
			// float DiffuseColour[3];
			float BaseColour[4];
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

	class Model
	{
	public:
		// Model() {};
		void SetTransform(glm::mat4 transform);

	public:
		std::string m_Name;

		std::vector<Ref<VertexArray>> m_VertexArrays;

		std::vector<Ref<PBRMaterial>> m_Materials;
		std::vector<Ref<Mesh>> m_Meshes;

		glm::mat4 m_Transform = glm::mat4(1.0f);
		glm::mat3 m_NormalMatrix = glm::mat4(1.0f);
	};

	class ModelManager
	{
	public:
		static Ref<Model> LoadModel(const char* filepath);

	private:
		static Ref<PBRMaterial> LoadPBRMaterial(std::ifstream& fileStream, const std::string& directoryPath);
		static void GenerateVertexArray(Ref<Model>);

	private:
		static std::vector<Ref<Model>> m_Models;
	};

}
