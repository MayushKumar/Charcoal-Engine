#include <iostream>
#include <string>
#include <fstream>
#include <cstring>

#include <tiny_obj_loader.h>

#define DEBUG_WITHOUT_INPUT 0

#if defined CH_PLATFORM_WINDOWS
	#define DEBUG_BREAK() __debugbreak()
#elif defined CH_PLATFORM_LINUX
	#define DEBUG_BREAK() asm("int $3")
#endif

enum class ShaderDataType // From "Charcoal/Renderer/Buffer.h"
{
	None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
};

enum class TextureType // From "Charcoal/Renderer/Model.h"
{
	None = 0, Albedo, Normal, Metallic, Roughness
};

enum class VertexAttribType // From "Charcoal/Renderer/Model.h" Mesh::VertexAttrib::VertexAttribType
{
	None = 0, Position, Normal, TexCoord
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

void LoadWavefrontObject(std::string& filePath);
void WriteToFile(CMFFormat* format);

int main(int argc, char* argv[])
{

#if DEBUG_WITHOUT_INPUT
#else
	if (argc < 2)
	{
		std::cout << "ERROR: Provide a file as a command line argument!" << std::endl;
		std::cin.get();
		return -1;
	}
#endif

#if DEBUG_WITHOUT_INPUT
	std::string filePath = "models/cerberus/Cerberus.obj";
#else
	std::string filePath = std::string(argv[1]);
#endif

	std::string fileExt;
	fileExt = filePath.substr(filePath.find_last_of('.'), filePath.length());

	if (!fileExt.compare(".obj"))
		LoadWavefrontObject(filePath);
	//else if (!fileExt.compare(".glb"))
	//	LoadBinaryGltfObject(filePath);
	//else if (!fileExt.compare(".gltf"))
	//	LoadASCIIGltfObject(filePath);
	else
	{
		std::cout << "ERROR: File extension not supported!" << std::endl;
		std::cin.get();
		return -1;
	}

}

void LoadWavefrontObject(std::string& filePath)
{
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> tinyobjmaterials;
	std::string warn, err;
	tinyobj::attrib_t attrib;

	int slashPos = filePath.find_last_of("/\\");
	std::string mtlBaseDir = filePath.substr(0, slashPos == std::string::npos ? 0 : slashPos);
	std::string fileName = filePath.substr(filePath.find_last_of("/\\") + 1, filePath.find_last_of(".") - (filePath.find_last_of("/\\")) - 1);

	bool success = tinyobj::LoadObj(&attrib, &shapes, &tinyobjmaterials, &warn, &err, filePath.c_str(), mtlBaseDir.c_str());

	if (!warn.empty())
	{
		std::cout << "WARNING loading Wavefront object: " << warn << std::endl;
	}
	if (!err.empty())
	{
		std::cout << "ERROR loading Wavefront object: " << err << std::endl;
	}
	if (!success)
	{
		std::cout << "Could not load file!" << std::endl;
		std::cin.get();
		DEBUG_BREAK();
	}

	//////////////// MATERIALS ////////////////

	std::vector<CMFFormat::Material> materials;
	std::vector<std::vector<CMFFormat::Material::Texture>> texturesBuffers;
	std::vector<uint32_t> textureIndicesMap; // Maps tinyobjloader texture indices to the original indices beingh stored in the texture buffers

	for (uint32_t i = 0; i < tinyobjmaterials.size(); i++)
	{
		CMFFormat::Material material;
		auto& tinyobjmaterial = tinyobjmaterials[i];

		material.NameSize = tinyobjmaterial.name.size() + 1;
		material.Name = new char[material.NameSize];
		strcpy(material.Name, tinyobjmaterial.name.c_str());

		material.ID = i;

		std::vector<CMFFormat::Material::Texture> textures;

		if (!tinyobjmaterials[i].diffuse_texname.empty()) 
		{
			CMFFormat::Material::Texture tex = CMFFormat::Material::Texture();
			tex.TextureType = (uint32_t)TextureType::Albedo;
			tex.RelativeFilepathSize = tinyobjmaterial.diffuse_texname.size() + 1;
			tex.RelativeFilepath = new char[tex.RelativeFilepathSize];
			strcpy(tex.RelativeFilepath, tinyobjmaterial.diffuse_texname.c_str());
			textures.push_back(tex);
			//tinyobjmaterial->AlbedoMap = Texture2D::Create(mtlBaseDir + "/" + tinyobjmaterial.diffuse_texname);
		}
		if (!tinyobjmaterials[i].normal_texname.empty())
		{
			CMFFormat::Material::Texture tex;
			tex.TextureType = (uint32_t)TextureType::Normal;
			tex.RelativeFilepathSize = tinyobjmaterial.normal_texname.size() + 1;
			tex.RelativeFilepath = new char[tex.RelativeFilepathSize];
			strcpy(tex.RelativeFilepath, tinyobjmaterial.normal_texname.c_str());
			textures.push_back(tex);
		}
		if (!tinyobjmaterials[i].metallic_texname.empty())
		{
			CMFFormat::Material::Texture tex;
			tex.TextureType = (uint32_t)TextureType::Metallic;
			tex.RelativeFilepathSize = tinyobjmaterial.metallic_texname.size() + 1;
			tex.RelativeFilepath = new char[tex.RelativeFilepathSize];
			strcpy(tex.RelativeFilepath, tinyobjmaterial.metallic_texname.c_str());
			textures.push_back(tex);
		}
		if (!tinyobjmaterials[i].roughness_texname.empty())
		{
			CMFFormat::Material::Texture tex;
			tex.TextureType = (uint32_t)TextureType::Roughness;
			tex.RelativeFilepathSize = tinyobjmaterial.roughness_texname.size() + 1;
			tex.RelativeFilepath = new char[tex.RelativeFilepathSize];
			strcpy(tex.RelativeFilepath, tinyobjmaterial.roughness_texname.c_str());
			textures.push_back(tex);
		}
	
		texturesBuffers.push_back(textures);

		material.AmbientColour[0] = tinyobjmaterials[i].ambient[0];
		material.AmbientColour[1] = tinyobjmaterials[i].ambient[1];
		material.AmbientColour[2] = tinyobjmaterials[i].ambient[2];

		material.DiffuseColour[0] = tinyobjmaterials[i].diffuse[0];
		material.DiffuseColour[1] = tinyobjmaterials[i].diffuse[1];
		material.DiffuseColour[2] = tinyobjmaterials[i].diffuse[2];
	
		material.MetallicFactor = tinyobjmaterials[i].metallic;
		material.RoughnessFactor = tinyobjmaterials[i].roughness;

		material.TextureCount = texturesBuffers[i].size();
		material.Textures = texturesBuffers[i].data();

		materials.push_back(material);
	}

	//////////////// GEOMETRY ////////////////

	std::vector<CMFFormat::Mesh> meshes;

	//Attribs
	std::vector<std::vector<CMFFormat::VertexAttrib>> vertexAttribs;
	std::vector<std::vector<float>> positionsBuffers;
	std::vector<std::vector<float>> normalsBuffers;
	std::vector<std::vector<float>> texCoordsBuffers;
	std::vector<std::vector<uint32_t>> indexBuffers;

	for (uint32_t i = 0; i < shapes.size(); i++)
	{
		auto& tinyobjIndices = shapes[i].mesh.indices;

		const uint32_t materialID = shapes[i].mesh.material_ids[0];
		for (int ID : shapes[i].mesh.material_ids)
		{
			if (ID != materialID)
			{
				std::cout << "ERROR: Consflicting Material IDs within the same Mesh";
				std::cin.get();
			}
		}

		CMFFormat::Mesh mesh;
		mesh.NameSize = shapes[i].name.size() + 1;
		mesh.Name = (char*)shapes[i].name.c_str();

		mesh.MaterialID = materialID;

		std::vector<float> positions;
		std::vector<float> normals;
		std::vector<float> texCoords;

		std::vector<uint32_t> indices;
		std::vector<std::vector<uint32_t>> texCoordIndexPerVertex;
		std::vector<std::vector<uint32_t>> normalIndicesPerVertex;
		std::vector<uint32_t> offsetFromOriginalVertex;
		std::vector<uint32_t> cumulativeVertices;

		indices.resize(tinyobjIndices.size());
		texCoordIndexPerVertex.resize(attrib.vertices.size() / 3);
		normalIndicesPerVertex.resize(attrib.vertices.size() / 3);
		offsetFromOriginalVertex.resize(tinyobjIndices.size());

		for (uint32_t j = 0; j < tinyobjIndices.size(); j++)
		{

			uint32_t vertexIndex = tinyobjIndices[j].vertex_index;
			uint32_t normalIndex = tinyobjIndices[j].normal_index;
			uint32_t texCoordIndex = tinyobjIndices[j].texcoord_index >= 0 ? tinyobjIndices[j].texcoord_index : 0;

			if (texCoordIndexPerVertex[vertexIndex].size())
			{
				bool exists = false;
				for (uint32_t k = 0; k < texCoordIndexPerVertex[vertexIndex].size(); k++)
				{
					if (texCoordIndexPerVertex[vertexIndex][k] == texCoordIndex)
					{
						//Vertex Exists
						offsetFromOriginalVertex[j] = k;
						exists = true;
						break;
					}
				}
				if (!exists)
				{
					//Duplicate Vertex
					offsetFromOriginalVertex[j] = texCoordIndexPerVertex[vertexIndex].size();
					texCoordIndexPerVertex[vertexIndex].push_back(texCoordIndex);
					normalIndicesPerVertex[vertexIndex].push_back(normalIndex);
				}
			}
			else
			{
				//New Vertex
				texCoordIndexPerVertex[vertexIndex].push_back(texCoordIndex);
				normalIndicesPerVertex[vertexIndex].push_back(normalIndex);
				offsetFromOriginalVertex[j] = 0;
			}

		}

		cumulativeVertices.resize(texCoordIndexPerVertex.size());

		//Appending offsets
		uint32_t generatedVerticesCount = 0;
		for (uint32_t j = 0; j < texCoordIndexPerVertex.size(); j++)
		{
			for (uint32_t k = 0; k < texCoordIndexPerVertex[j].size(); k++)
			{
				uint32_t texCoordIndex = texCoordIndexPerVertex[j][k];
				uint32_t normalIndex = normalIndicesPerVertex[j][k];

				positions.push_back(attrib.vertices[j * 3]);
				positions.push_back(attrib.vertices[j * 3 + 1]);
				positions.push_back(attrib.vertices[j * 3 + 2]);

				normals.push_back(attrib.normals[normalIndex * 3]);
				normals.push_back(attrib.normals[normalIndex * 3 + 1]);
				normals.push_back(attrib.normals[normalIndex * 3 + 2]);

				texCoords.push_back(attrib.texcoords[texCoordIndex * 2]);
				texCoords.push_back(attrib.texcoords[texCoordIndex * 2 + 1]);
			}
			if (j == 0)
			{
				cumulativeVertices[j] = 0;
			}
			else
			{
				generatedVerticesCount += texCoordIndexPerVertex[j - 1].size();
				cumulativeVertices[j] = generatedVerticesCount;
			}
		}

		positionsBuffers.push_back(positions);
		normalsBuffers.push_back(normals);
		texCoordsBuffers.push_back(texCoords);

		std::vector<CMFFormat::VertexAttrib> attribs;

		CMFFormat::VertexAttrib positionsAttrib;
		positionsAttrib.NameSize = 11;
		positionsAttrib.Name = "a_Position";
		positionsAttrib.ShaderDataType = (uint32_t)ShaderDataType::Float3;
		positionsAttrib.VertexAttribType = (uint32_t)VertexAttribType::Position;
		positionsAttrib.BufferSize = positions.size() * sizeof(float);
		positionsAttrib.Buffer = positionsBuffers[i].data();

		CMFFormat::VertexAttrib normalsAttrib;
		normalsAttrib.NameSize = 9;
		normalsAttrib.Name = "a_Normal";
		normalsAttrib.ShaderDataType = (uint32_t)ShaderDataType::Float3;
		normalsAttrib.VertexAttribType = (uint32_t)VertexAttribType::Normal;
		normalsAttrib.BufferSize = normals.size() * sizeof(float);
		normalsAttrib.Buffer = normalsBuffers[i].data();

		CMFFormat::VertexAttrib texCoordsAttrib;
		texCoordsAttrib.NameSize = 11;
		texCoordsAttrib.Name = "a_TexCoord";
		texCoordsAttrib.ShaderDataType = (uint32_t)ShaderDataType::Float2;
		texCoordsAttrib.VertexAttribType = (uint32_t)VertexAttribType::TexCoord;
		texCoordsAttrib.BufferSize = texCoords.size() * sizeof(float);
		texCoordsAttrib.Buffer = texCoordsBuffers[i].data();

		attribs.push_back(positionsAttrib);
		attribs.push_back(normalsAttrib);
		attribs.push_back(texCoordsAttrib);

		vertexAttribs.push_back(attribs);

		mesh.VertexAttribCount = vertexAttribs[i].size();
		mesh.VertexAttribs = vertexAttribs[i].data();


		for (uint32_t i = 0; i < tinyobjIndices.size(); i++)
		{
			uint32_t index = cumulativeVertices[tinyobjIndices[i].vertex_index] + offsetFromOriginalVertex[i];
			indices[i] = index;
		}

		indexBuffers.push_back(indices);

		mesh.IndexBufferSize = indexBuffers[i].size() * sizeof(uint32_t);
		mesh.IndexBuffer = indexBuffers[i].data();

		meshes.push_back(mesh);

	}
	////////////////// WRITING TO FORMAT STRUCT //////////////////

	CMFFormat* format = new CMFFormat();

	format->NameSize = fileName.size() + 1;
	format->Name = new char[format->NameSize];
	strcpy(format->Name, fileName.c_str());

	format->MaterialCount = materials.size();
	format->Materials = materials.data();

	format->MeshCount = meshes.size();
	format->Meshes = meshes.data();

	//format->vertexAttribCount = 3; // TODO: determine vertexAttribCount from model
	//format->vertexAttribs = new CMFFormat::VertexAttrib[format->vertexAttribCount];
	//
	//format->vertexAttribs[0].nameSize = 11;
	//format->vertexAttribs[0].name = "a_Position";
	//format->vertexAttribs[0].shaderDataType = (uint32_t)ShaderDataType::Float3;
	//
	//format->vertexAttribs[1].nameSize = 11;
	//format->vertexAttribs[1].name = "a_Normal";
	//format->vertexAttribs[1].shaderDataType = (uint32_t)ShaderDataType::Float3;
	//
	//format->vertexAttribs[2].nameSize = 11;
	//format->vertexAttribs[2].name = "a_TexCoord";
	//format->vertexAttribs[2].shaderDataType = (uint32_t)ShaderDataType::Float2;
	//
	//format->vertexBufferSize = vertices.size() * sizeof(Vertex);
	//format->vertexBuffer = vertices.data();
	//
	//format->indexBufferSize = indices.size() * sizeof(uint32_t);
	//format->indexBuffer = indices.data();

	//////////////////////////////////////////////////////////////

	WriteToFile(format);

	// CLEANUP

	for (auto& textureBuffer : texturesBuffers)
	{
		for(auto& tex: textureBuffer)
			delete[] tex.RelativeFilepath;
	}

	for (auto& material : materials)
		delete[] material.Name;

	delete[] format->Name;
	delete format;
}

void WriteToFile(CMFFormat* format)
{
	std::ofstream fileStream;

	char* fullFileName = new char[format->NameSize + 4];
	strcpy(fullFileName, format->Name);
	strcat(fullFileName, ".cmf");

	fileStream.open(fullFileName, std::ios::binary);

	fileStream.write((const char*)&format->HeaderSize, sizeof(format->HeaderSize));
	fileStream.write(format->Header, format->HeaderSize);
	fileStream.write((const char*)&format->NameSize, sizeof(format->NameSize));
	fileStream.write(format->Name, format->NameSize);

	fileStream.write((const char*)&format->MaterialCount, sizeof(format->MaterialCount));
	for (uint32_t i = 0; i < format->MaterialCount; i++)
	{
		fileStream.write((const char*)&format->Materials[i].NameSize, sizeof(format->Materials[i].NameSize));
		fileStream.write(format->Materials[i].Name, format->Materials[i].NameSize);

		fileStream.write((const char*)&format->Materials[i].ID, sizeof(format->Materials[i].ID));
		fileStream.write((const char*)&format->Materials[i].AmbientColour, sizeof(format->Materials[i].AmbientColour));
		fileStream.write((const char*)&format->Materials[i].DiffuseColour, sizeof(format->Materials[i].DiffuseColour));
		fileStream.write((const char*)&format->Materials[i].MetallicFactor, sizeof(format->Materials[i].MetallicFactor));
		fileStream.write((const char*)&format->Materials[i].RoughnessFactor, sizeof(format->Materials[i].RoughnessFactor));

		fileStream.write((const char*)&format->Materials[i].TextureCount, sizeof(format->Materials[i].TextureCount));
		for (uint32_t j = 0; j < format->Materials[i].TextureCount; j++)
		{
			fileStream.write((const char*)&format->Materials[i].Textures[j].TextureType, sizeof(format->Materials[i].Textures[j].TextureType));
			fileStream.write((const char*)&format->Materials[i].Textures[j].RelativeFilepathSize, sizeof(format->Materials[i].Textures[j].RelativeFilepathSize));
			fileStream.write(format->Materials[i].Textures[j].RelativeFilepath, format->Materials[i].Textures[j].RelativeFilepathSize);
		}
	}

	fileStream.write((const char*)&format->MeshCount, sizeof(format->MeshCount));
	for (uint32_t i = 0; i < format->MeshCount; i++)
	{
		fileStream.write((const char*)&format->Meshes[i].NameSize, sizeof(format->Meshes[i].NameSize));
		fileStream.write(format->Meshes[i].Name, format->Meshes[i].NameSize);

		fileStream.write((const char*)&format->Meshes[i].MaterialID, sizeof(format->Meshes[i].MaterialID));

		fileStream.write((const char*)&format->Meshes[i].VertexAttribCount, sizeof(format->Meshes[i].VertexAttribCount));
		for (uint32_t j = 0; j < format->Meshes[i].VertexAttribCount; j++)
		{
			fileStream.write((const char*)&format->Meshes[i].VertexAttribs[j].NameSize, sizeof(format->Meshes[i].VertexAttribs[j].NameSize));
			fileStream.write(format->Meshes[i].VertexAttribs[j].Name, format->Meshes[i].VertexAttribs[j].NameSize);

			fileStream.write((const char*)&format->Meshes[i].VertexAttribs[j].ShaderDataType, sizeof(format->Meshes[i].VertexAttribs[j].ShaderDataType));
			fileStream.write((const char*)&format->Meshes[i].VertexAttribs[j].VertexAttribType, sizeof(format->Meshes[i].VertexAttribs[j].VertexAttribType));

			fileStream.write((const char*)&format->Meshes[i].VertexAttribs[j].BufferSize, sizeof(format->Meshes[i].VertexAttribs[j].BufferSize));
			fileStream.write((const char*)format->Meshes[i].VertexAttribs[j].Buffer, format->Meshes[i].VertexAttribs[j].BufferSize);
		}
		fileStream.write((const char*)&format->Meshes[i].IndexBufferSize, sizeof(format->Meshes[i].IndexBufferSize));
		fileStream.write((const char*)format->Meshes[i].IndexBuffer, format->Meshes[i].IndexBufferSize);
	}

	fileStream.write((const char*)&format->FooterSize, sizeof(format->FooterSize));
	fileStream.write(format->Footer, format->FooterSize);
	//fileStream.write((const char*)&format->vertexAttribCount, sizeof(format->vertexAttribCount));
	//for (uint32_t i = 0; i < format->vertexAttribCount; i++)
	//{
	//	fileStream.write((const char*)&format->vertexAttribs[i].nameSize, sizeof(format->vertexAttribs[i].nameSize));
	//	fileStream.write(format->vertexAttribs[i].name, format->vertexAttribs[i].nameSize);
	//	fileStream.write((const char*)&format->vertexAttribs[i].shaderDataType, sizeof(format->vertexAttribs[i].shaderDataType));
	//}
	//fileStream.write((const char*)&format->vertexBufferSize, sizeof(format->vertexBufferSize));
	//fileStream.write((const char*)format->vertexBuffer, format->vertexBufferSize);
	//fileStream.write((const char*)&format->indexBufferSize, sizeof(format->indexBufferSize));
	//fileStream.write((const char*)format->indexBuffer, format->indexBufferSize);

	fileStream.close();
}
