#include "chpch.h"
#include "Model.h"

#include <fstream>

namespace Charcoal
{

	Model::Model(const char* filePath)
	{
		LoadModel(filePath);
	}

	void Model::LoadModel(const char* filePath)
	{
		std::string directoryPath = filePath;
		directoryPath = directoryPath.substr(0, directoryPath.find_last_of("/\\") + 1);

		std::ifstream fileStream;
		fileStream.open(filePath, std::ios::binary);

		uint32_t headerSize = 0;
		fileStream.read((char*)&headerSize, sizeof(CMFFormat::HeaderSize));
		char* header = new char[headerSize];
		fileStream.read(header, headerSize);
		delete[] header;

		uint32_t nameSize = 0;
		fileStream.read((char*)&nameSize, sizeof(CMFFormat::NameSize));
		char* name = new char[nameSize];
		fileStream.read(name, nameSize);
		m_Name = name;
		delete[] name;

		uint32_t materialCount = 0;
		fileStream.read((char*)&materialCount, sizeof(CMFFormat::MaterialCount));

		m_Materials.resize(materialCount);

		for (uint32_t i = 0; i < materialCount; i++)
		{
			Ref<Material> material = CreateRef<Material>();

			uint32_t nameSize = 0;
			fileStream.read((char*)&nameSize, sizeof(CMFFormat::Material::NameSize));
			char* name = new char[nameSize];
			fileStream.read(name, nameSize);
			material->m_Name = name;
			delete[] name;

			uint32_t ID = 0;
			fileStream.read((char*)&ID, sizeof(CMFFormat::Material::ID));

			fileStream.read((char*)material->m_AmbientColour, sizeof(CMFFormat::Material::AmbientColour));
			fileStream.read((char*)material->m_DiffuseColour, sizeof(CMFFormat::Material::DiffuseColour));
			fileStream.read((char*)&material->m_MetallicFactor, sizeof(CMFFormat::Material::MetallicFactor));
			fileStream.read((char*)&material->m_RoughnessFactor, sizeof(CMFFormat::Material::RoughnessFactor));

			uint32_t textureCount = 0;
			fileStream.read((char*)&textureCount, sizeof(CMFFormat::Material::TextureCount));
			material->m_Textures.resize(textureCount);
			material->m_TextureTypes.resize(textureCount);

			for (uint32_t j = 0; j < textureCount; j++)
			{
				TextureType type = TextureType::None;
				fileStream.read((char*)&type, sizeof(CMFFormat::Material::Texture::TextureType));
				material->m_TextureTypes[j] = type;

				uint32_t relativeFilePathSize = 0;
				fileStream.read((char*)&relativeFilePathSize, sizeof(CMFFormat::Material::Texture::RelativeFilepathSize));
				char* relativeFilePath = new char[relativeFilePathSize];
				fileStream.read(relativeFilePath, relativeFilePathSize);

				Ref<Texture2D> texture = Texture2D::Create(directoryPath + relativeFilePath);
				material->m_Textures[j] = texture;

				delete[] relativeFilePath;
			}

			m_Materials[i] = material;
		}

		uint32_t meshCount = 0;
		fileStream.read((char*)&meshCount, sizeof(CMFFormat::MeshCount));
		m_Meshes.resize(meshCount);

		for (uint32_t i = 0; i < meshCount; i++)
		{
			Ref<Mesh> mesh = CreateRef<Mesh>();

			uint32_t nameSize = 0;
			fileStream.read((char*)&nameSize, sizeof(CMFFormat::Mesh::NameSize));
			char* name = new char[nameSize];
			fileStream.read(name, nameSize);
			mesh->Name = name;
			delete[] name;

			uint32_t materialIndex = 0;
			fileStream.read((char*)&materialIndex, sizeof(CMFFormat::Mesh::MaterialID));

			uint32_t vertexAttribCount = 0;
			fileStream.read((char*)&vertexAttribCount, sizeof(CMFFormat::Mesh::VertexAttribCount));
			mesh->m_VertexAttribs.resize(vertexAttribCount);
			for (uint32_t j = 0; j < vertexAttribCount; j++)
			{
				Mesh::VertexAttrib vertexAttrib;

				uint32_t nameSize = 0;
				fileStream.read((char*)&nameSize, sizeof(CMFFormat::VertexAttrib::NameSize));
				char* name = new char[nameSize];
				fileStream.read(name, nameSize);
				vertexAttrib.Name = name;
				delete[] name;

				ShaderDataType shaderDataType = ShaderDataType::None;
				fileStream.read((char*)&shaderDataType, sizeof(CMFFormat::VertexAttrib::ShaderDataType));
				vertexAttrib.DataType = shaderDataType;

				Mesh::VertexAttrib::VertexAttribType type = Mesh::VertexAttrib::VertexAttribType::None;
				fileStream.read((char*)&type, sizeof(Mesh::VertexAttrib::VertexAttribType));
				vertexAttrib.AttribType = type;

				uint32_t bufferSize = 0;
				fileStream.read((char*)&bufferSize, sizeof(CMFFormat::VertexAttrib::BufferSize));

				vertexAttrib.Buffer.resize(bufferSize);
				fileStream.read(vertexAttrib.Buffer.data(), bufferSize);

				mesh->m_VertexAttribs[j] = vertexAttrib;
			}

			uint32_t indexBufferSize = 0;
			fileStream.read((char*)&indexBufferSize, sizeof(CMFFormat::Mesh::IndexBufferSize));
			mesh->m_IndexBufferData.resize(indexBufferSize / sizeof(uint32_t));

			fileStream.read((char*)mesh->m_IndexBufferData.data(), indexBufferSize);

			m_Meshes[i] = mesh;
		}

		uint32_t footerSize = 0;
		fileStream.read((char*)&footerSize, sizeof(CMFFormat::FooterSize));
		char* footer = new char[footerSize];
		fileStream.read(footer, footerSize);
		delete[] footer;

		GenerateVertexArray();
	}

	void Model::GenerateVertexArray()
	{
		m_VertexArrays.resize(m_Meshes.size());

		for (uint32_t i = 0; i < m_Meshes.size(); i++)
		{
			m_VertexArrays[i] = VertexArray::Create();

			for (uint32_t j = 0; j < m_Meshes[i]->m_VertexAttribs.size(); j++)
			{
				Ref<VertexBuffer> vbo = VertexBuffer::Create(m_Meshes[i]->m_VertexAttribs[j].Buffer.size(), (float*)m_Meshes[i]->m_VertexAttribs[j].Buffer.data());
				BufferLayout layout = {
					{ m_Meshes[i]->m_VertexAttribs[j].DataType, m_Meshes[i]->m_VertexAttribs[j].Name }
				};
				vbo->SetLayout(layout);
				m_VertexArrays[i]->AddVertexBuffer(vbo, j);
			}

			Ref<IndexBuffer> ibo = IndexBuffer::Create(m_Meshes[i]->m_IndexBufferData.size() * sizeof(uint32_t), m_Meshes[i]->m_IndexBufferData.data());
			m_VertexArrays[i]->SetIndexBuffer(ibo);
		}
	}

}