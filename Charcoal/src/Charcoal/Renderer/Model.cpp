#include "chpch.h"
#include "Model.h"

#include "ShaderManager.h"

#include <fstream>

namespace Charcoal
{

	void Model::SetTransform(glm::mat4 transform)
	{
		m_Transform = transform;
		m_NormalMatrix = glm::transpose(glm::inverse(glm::mat3(m_Transform)));
	}

	std::vector<Ref<Model>> ModelManager::m_Models;

	Ref<PBRMaterial> ModelManager::LoadPBRMaterial(std::ifstream& fileStream, const std::string& directoryPath)
	{
		Ref<PBRMaterial> material = CreateRef<PBRMaterial>();
		material->m_Shader = ShaderManager::GetShader("PBR");
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

		for (uint32_t j = 0; j < textureCount; j++)
		{
			Material::TextureType type = Material::TextureType::None;
			fileStream.read((char*)&type, sizeof(CMFFormat::Material::Texture::TextureType));

			uint32_t relativeFilePathSize = 0;
			fileStream.read((char*)&relativeFilePathSize, sizeof(CMFFormat::Material::Texture::RelativeFilepathSize));
			char* relativeFilePath = new char[relativeFilePathSize];
			fileStream.read(relativeFilePath, relativeFilePathSize);

			Ref<Texture2D> texture = Texture2D::Create({TextureInternalFormat::RGB,
					TextureFormat::RGB, TextureDataType::UByte}, directoryPath + relativeFilePath);

			switch (type)
			{
			case Material::TextureType::Albedo:
				material->m_AlbedoMap = texture;
				break;
			case Material::TextureType::Normal:
				material->m_NormalMap = texture;
				break;
			case Material::TextureType::Metallic:
				material->m_MetallicMap = texture;
				break;
			case Material::TextureType::Roughness:
				material->m_RoughnessMap = texture;
				break;
			case Material::TextureType::None:
				CH_CORE_ERROR("Texture type not supported for {0}", directoryPath + relativeFilePath);
				break;
			}
			
			delete[] relativeFilePath;
		}
		return material;
	}

	void ModelManager::GenerateVertexArray(Ref<Model> model)
	{
		model->m_VertexArrays.resize(model->m_Meshes.size());

		for (uint32_t i = 0; i < model->m_Meshes.size(); i++)
		{
			model->m_VertexArrays[i] = VertexArray::Create();

			for (uint32_t j = 0; j < model->m_Meshes[i]->m_VertexAttribs.size(); j++)
			{
				Ref<VertexBuffer> vbo = VertexBuffer::Create(model->m_Meshes[i]->m_VertexAttribs[j].Buffer.size(), (float*)model->m_Meshes[i]->m_VertexAttribs[j].Buffer.data());
				BufferLayout layout = {
					{ model->m_Meshes[i]->m_VertexAttribs[j].DataType, model->m_Meshes[i]->m_VertexAttribs[j].Name }
				};
				vbo->SetLayout(layout);
				model->m_VertexArrays[i]->AddVertexBuffer(vbo, j);
			}

			Ref<IndexBuffer> ibo = IndexBuffer::Create(model->m_Meshes[i]->m_IndexBufferData.size() * sizeof(uint32_t), model->m_Meshes[i]->m_IndexBufferData.data());
			model->m_VertexArrays[i]->SetIndexBuffer(ibo);
		}
	}

	Ref<Model> ModelManager::LoadModel(const char* filepath)
	{
		std::string directoryPath = filepath;
		directoryPath = directoryPath.substr(0, directoryPath.find_last_of("/\\") + 1);

		std::ifstream fileStream;
		fileStream.open(filepath, std::ios::binary);
		if(!fileStream)
		{
			CH_CORE_ERROR("Could not open the Model file: {0}", filepath);
			return nullptr;
		}

		Ref<Model> model = CreateRef<Model>();

		uint32_t headerSize = 0;
		fileStream.read((char*)&headerSize, sizeof(CMFFormat::HeaderSize));
		char* header = new char[headerSize];
		fileStream.read(header, headerSize);
		delete[] header;

		uint32_t nameSize = 0;
		fileStream.read((char*)&nameSize, sizeof(CMFFormat::NameSize));
		char* name = new char[nameSize];
		fileStream.read(name, nameSize);
		model->m_Name = name;
		delete[] name;

		uint32_t materialCount = 0;
		fileStream.read((char*)&materialCount, sizeof(CMFFormat::MaterialCount));

		model->m_Materials.resize(materialCount);

		for (uint32_t i = 0; i < materialCount; i++)
		{
			model->m_Materials[i] = LoadPBRMaterial(fileStream, directoryPath);
		}

		uint32_t meshCount = 0;
		fileStream.read((char*)&meshCount, sizeof(CMFFormat::MeshCount));
		model->m_Meshes.resize(meshCount);

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

			model->m_Meshes[i] = mesh;
		}

		uint32_t footerSize = 0;
		fileStream.read((char*)&footerSize, sizeof(CMFFormat::FooterSize));
		char* footer = new char[footerSize];
		fileStream.read(footer, footerSize);
		delete[] footer;

		GenerateVertexArray(model);

		m_Models.push_back(model);
		return model;
	}

}
