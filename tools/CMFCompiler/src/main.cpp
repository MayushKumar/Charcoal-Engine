#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>
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

void LoadWavefrontObject(std::string& filePath);
void WriteToFile(CMFFormat* format);
TangentVectors CalculateTangentVectors(
	glm::vec3 e1, 
	glm::vec3 e2,
	glm::vec2 duv1,
	glm::vec2 duv2
	);

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
	std::string filePath = "../../Sandbox/assets/models/plane/plane.obj";
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
	std::vector<std::vector<float>> tangentsBuffers;
	std::vector<std::vector<float>> bitangentsBuffers;
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
				std::cout << "ERROR: Conflicting Material IDs within the same Mesh";
				std::cin.get();
			}
		}

		CMFFormat::Mesh mesh;
		mesh.NameSize = shapes[i].name.size() + 1;
		mesh.Name = (char*)shapes[i].name.c_str();

		mesh.MaterialID = materialID;

		std::vector<float> positions;
		std::vector<float> normals;
		std::vector<float> tangents;
		std::vector<float> bitangents;
		std::vector<float> texCoords;

		std::vector<uint32_t> indices;
		std::vector<std::vector<uint32_t>> texCoordIndicesPerVertex;
		std::vector<std::vector<uint32_t>> normalIndicesPerVertex;
		std::vector<std::vector<uint32_t>> tangentsIndicesPerVertex;
		std::vector<uint32_t> offsetFromOriginalVertex;
		std::vector<uint32_t> cumulativeVertices;

		indices.resize(tinyobjIndices.size());
		texCoordIndicesPerVertex.resize(attrib.vertices.size() / 3);
		normalIndicesPerVertex.resize(attrib.vertices.size() / 3);
		tangentsIndicesPerVertex.resize(attrib.vertices.size() / 3);
		offsetFromOriginalVertex.resize(tinyobjIndices.size());

		std::vector<TangentVectors> tempTangentsBuffer;
		for (uint32_t j = 0; j < tinyobjIndices.size(); j++)
		{

			uint32_t vertexIndex = tinyobjIndices[j].vertex_index;
			uint32_t normalIndex = tinyobjIndices[j].normal_index;
			uint32_t tangentsIndex = j / 3; // Increments tangents index every 3 indices
			uint32_t texCoordIndex = tinyobjIndices[j].texcoord_index >= 0 ? tinyobjIndices[j].texcoord_index : 0;

			if (texCoordIndicesPerVertex[vertexIndex].size())
			{
				bool exists = false;
				for (uint32_t k = 0; k < texCoordIndicesPerVertex[vertexIndex].size(); k++)
				{
					if (texCoordIndicesPerVertex[vertexIndex][k] == texCoordIndex && normalIndicesPerVertex[vertexIndex][k] == normalIndex)
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
					offsetFromOriginalVertex[j] = texCoordIndicesPerVertex[vertexIndex].size();
					texCoordIndicesPerVertex[vertexIndex].push_back(texCoordIndex);
					normalIndicesPerVertex[vertexIndex].push_back(normalIndex);
					tangentsIndicesPerVertex[vertexIndex].push_back(tangentsIndex);
				}
			}
			else
			{
				//New Vertex
				texCoordIndicesPerVertex[vertexIndex].push_back(texCoordIndex);
				normalIndicesPerVertex[vertexIndex].push_back(normalIndex);
				tangentsIndicesPerVertex[vertexIndex].push_back(tangentsIndex);
				offsetFromOriginalVertex[j] = 0;
			}
			// Calculate Tangent Vectors
			if (j % 3 == 0)
			{
				tempTangentsBuffer.push_back(CalculateTangentVectors(
												 {attrib.vertices[tinyobjIndices[j + 1].vertex_index * 3]     - attrib.vertices[tinyobjIndices[j].vertex_index * 3],
												  attrib.vertices[tinyobjIndices[j + 1].vertex_index * 3 + 1] - attrib.vertices[tinyobjIndices[j].vertex_index * 3 + 1],
												  attrib.vertices[tinyobjIndices[j + 1].vertex_index * 3 + 2] - attrib.vertices[tinyobjIndices[j].vertex_index * 3 + 2]},
												 {attrib.vertices[tinyobjIndices[j + 2].vertex_index * 3]     - attrib.vertices[tinyobjIndices[j].vertex_index * 3],
												  attrib.vertices[tinyobjIndices[j + 2].vertex_index * 3 + 1] - attrib.vertices[tinyobjIndices[j].vertex_index * 3 + 1],
												  attrib.vertices[tinyobjIndices[j + 2].vertex_index * 3 + 2] - attrib.vertices[tinyobjIndices[j].vertex_index * 3 + 2]},

												 {attrib.texcoords[tinyobjIndices[j + 1].texcoord_index * 2]     - attrib.texcoords[tinyobjIndices[j].texcoord_index * 2],
												  attrib.texcoords[tinyobjIndices[j + 1].texcoord_index * 2 + 1] - attrib.texcoords[tinyobjIndices[j].texcoord_index * 2 + 1]},
												 {attrib.texcoords[tinyobjIndices[j + 2].texcoord_index * 2]     - attrib.texcoords[tinyobjIndices[j].texcoord_index * 2],
												  attrib.texcoords[tinyobjIndices[j + 2].texcoord_index * 2 + 1] - attrib.texcoords[tinyobjIndices[j].texcoord_index * 2 + 1]}
					));
			}

		}

		cumulativeVertices.resize(texCoordIndicesPerVertex.size());

		//Appending offsets
		uint32_t generatedVerticesCount = 0;
		for (uint32_t j = 0; j < texCoordIndicesPerVertex.size(); j++)
		{
			for (uint32_t k = 0; k < texCoordIndicesPerVertex[j].size(); k++)
			{
				uint32_t texCoordIndex = texCoordIndicesPerVertex[j][k];
				uint32_t normalIndex = normalIndicesPerVertex[j][k];
				uint32_t tangentsIndex = tangentsIndicesPerVertex[j][k];

				positions.push_back(attrib.vertices[j * 3]);
				positions.push_back(attrib.vertices[j * 3 + 1]);
				positions.push_back(attrib.vertices[j * 3 + 2]);

				normals.push_back(attrib.normals[normalIndex * 3]);
				normals.push_back(attrib.normals[normalIndex * 3 + 1]);
				normals.push_back(attrib.normals[normalIndex * 3 + 2]);

				tangents.push_back(tempTangentsBuffer[tangentsIndex].Tangent[0]);
				tangents.push_back(tempTangentsBuffer[tangentsIndex].Tangent[1]);
				tangents.push_back(tempTangentsBuffer[tangentsIndex].Tangent[2]);

				bitangents.push_back(tempTangentsBuffer[tangentsIndex].Bitangent[0]);
				bitangents.push_back(tempTangentsBuffer[tangentsIndex].Bitangent[1]);
				bitangents.push_back(tempTangentsBuffer[tangentsIndex].Bitangent[2]);

				texCoords.push_back(attrib.texcoords[texCoordIndex * 2]);
				texCoords.push_back(attrib.texcoords[texCoordIndex * 2 + 1]);
			}
			if (j == 0)
			{
				cumulativeVertices[j] = 0;
			}
			else
			{
				generatedVerticesCount += texCoordIndicesPerVertex[j - 1].size();
				cumulativeVertices[j] = generatedVerticesCount;
			}
		}

		positionsBuffers.push_back(positions);
		normalsBuffers.push_back(normals);
		tangentsBuffers.push_back(tangents);
		bitangentsBuffers.push_back(bitangents);
		texCoordsBuffers.push_back(texCoords);

		std::vector<CMFFormat::VertexAttrib> attribs;

		CMFFormat::VertexAttrib positionsAttrib;
		positionsAttrib.NameSize = 11;
		positionsAttrib.Name = "a_Position";
		positionsAttrib.ShaderDataType = (uint32_t)ShaderDataType::Float3;
		positionsAttrib.VertexAttribType = (uint32_t)VertexAttribType::Position;
		positionsAttrib.BufferSize = positionsBuffers[i].size() * sizeof(float);
		positionsAttrib.Buffer = positionsBuffers[i].data();

		CMFFormat::VertexAttrib normalsAttrib;
		normalsAttrib.NameSize = 9;
		normalsAttrib.Name = "a_Normal";
		normalsAttrib.ShaderDataType = (uint32_t)ShaderDataType::Float3;
		normalsAttrib.VertexAttribType = (uint32_t)VertexAttribType::Normal;
		normalsAttrib.BufferSize = normalsBuffers[i].size() * sizeof(float);
		normalsAttrib.Buffer = normalsBuffers[i].data();

		CMFFormat::VertexAttrib tangentsAttrib;
		tangentsAttrib.NameSize = 10;
		tangentsAttrib.Name = "a_Tangent";
		tangentsAttrib.ShaderDataType = (uint32_t)ShaderDataType::Float3;
		tangentsAttrib.VertexAttribType = (uint32_t)VertexAttribType::Tangent;
		tangentsAttrib.BufferSize = tangentsBuffers[i].size() * sizeof(float);
		tangentsAttrib.Buffer = tangentsBuffers[i].data();

		CMFFormat::VertexAttrib bitangentsAttrib;
		bitangentsAttrib.NameSize = 13;
		bitangentsAttrib.Name = "a_Bitangents";
		bitangentsAttrib.ShaderDataType = (uint32_t)ShaderDataType::Float3;
		bitangentsAttrib.VertexAttribType = (uint32_t)VertexAttribType::Bitangent;
		bitangentsAttrib.BufferSize = bitangentsBuffers[i].size() * sizeof(float);
		bitangentsAttrib.Buffer = bitangentsBuffers[i].data();

		CMFFormat::VertexAttrib texCoordsAttrib;
		texCoordsAttrib.NameSize = 11;
		texCoordsAttrib.Name = "a_TexCoord";
		texCoordsAttrib.ShaderDataType = (uint32_t)ShaderDataType::Float2;
		texCoordsAttrib.VertexAttribType = (uint32_t)VertexAttribType::TexCoord;
		texCoordsAttrib.BufferSize = texCoordsBuffers[i].size() * sizeof(float);
		texCoordsAttrib.Buffer = texCoordsBuffers[i].data();

		attribs.push_back(positionsAttrib);
		attribs.push_back(normalsAttrib);
		attribs.push_back(tangentsAttrib);
		attribs.push_back(bitangentsAttrib);
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

	fileStream.close();
}

TangentVectors CalculateTangentVectors(
	glm::vec3 e1, 
	glm::vec3 e2,
	glm::vec2 duv1,
	glm::vec2 duv2
	)
{
	TangentVectors result;

	e1 = glm::normalize(e1);
	e2 = glm::normalize(e2);
	duv1 = glm::normalize(duv1);
	duv2 = glm::normalize(duv2);
	
	glm::mat3x2 edges = glm::transpose(glm::mat2x3(e1, e2));
	glm::mat2 uv = glm::transpose(glm::mat2(duv1, duv2));
	
	glm::mat3x2 TB = glm::inverse(uv) * edges;

	glm::vec3 T = glm::row(TB, 0);
	glm::vec3 B = glm::row(TB, 1);
	
	result.Tangent[0] = T.x;
	result.Tangent[1] = T.y;
	result.Tangent[2] = T.z;

	result.Bitangent[0] = B.x;
	result.Bitangent[1] = B.y;
	result.Bitangent[2] = B.z;

	// duv1[0] = du1 / duv1mag;
	// duv1[1] = dv1 / duv1mag;	
	// duv2[0] = du2 / duv2mag;
	// duv2[1] = dv2 / duv2mag;

	// float discr = 1 / ((duv1[0] * duv2[1]) - (duv2[0] * duv1[1]));

	// result.Tangent[0] = discr * ((e1[0] * duv2[1]) + (e2[0] * -duv1[1]));
	// result.Tangent[1] = discr * ((e1[1] * duv2[1]) + (e2[1] * -duv1[1]));
	// result.Tangent[2] = discr * ((e1[2] * duv2[1]) + (e2[2] * -duv1[1]));

	// result.Bitangent[0] = discr * ((e1[0] * -duv2[0]) + (e2[0] * duv1[0]));
	// result.Bitangent[1] = discr * ((e1[1] * -duv2[0]) + (e2[1] * duv1[0]));
	// result.Bitangent[2] = discr * ((e1[2] * -duv2[0]) + (e2[2] * duv1[0]));
	
	float test = glm::dot(T, B);
	
	return result;
}
