#include <string.h>
#include <iostream>
#include "tiny_gltf.h"

#include "defines.h"
#include <glm/glm.hpp>

struct ModelLoadingMetadata
{
	bool CalculateTangentSpace;
};

void ProcessModel(tinygltf::Model &model, CMFFormat &cmfformat, ModelLoadingMetadata &metadata);
void ProcessScene(tinygltf::Scene &scene, tinygltf::Model &model, CMFFormat &cmfformat, ModelLoadingMetadata &metadata);

void Cleanup(ModelLoadingMetadata &metadata);

struct LoadingData
{
	float* tangent_buffer;
	float* bitangent_buffer;
	float* modified_tex_coords;
};

static LoadingData s_LoadingData;

void LoadASCIIGLTF(std::string& filepath)
{

	std::string fileName = filepath.substr(filepath.find_last_of("/\\") + 1,
										   filepath.find_last_of(".") -
										   (filepath.find_last_of("/\\")) - 1);
	tinygltf::Model model;
	tinygltf::TinyGLTF loader;
	std::string err;
	std::string warn;
	bool ret = loader.LoadASCIIFromFile(&model, &err,
										&warn, filepath);

	if(!err.empty())
	{
		std::cout << "Error: " << err << "\n";
	}

	if(!warn.empty())
	{
		std::cout << "Warn: " << warn << "\n";
	}

	if(!ret)
	{
		std::cout << "Failed to parse gltf" << "\n";
	}

	CMFFormat cmfformat;
	cmfformat.NameSize = fileName.size() + 1;
	cmfformat.Name = fileName.c_str();

	ModelLoadingMetadata metadata = {true};

	ProcessModel(model, cmfformat, metadata);
	WriteToFile(cmfformat);
	Cleanup(metadata);
}

void ProcessModel(tinygltf::Model &model, CMFFormat &cmfformat, ModelLoadingMetadata &metadata)
{
	for(tinygltf::Scene &scene : model.scenes)
	{
		ProcessScene(scene, model, cmfformat, metadata);
	}
}

void ProcessScene(tinygltf::Scene &scene, tinygltf::Model &model, CMFFormat &cmfformat, ModelLoadingMetadata &metadata)
{
	// Should be a loop, but considering a primitive as a cmfmesh for now.
	// for(int nodeindex = 0; nodeindex <= scene.nodes.size(); nodeindex++)
	int nodeindex = 0;
	{
		tinygltf::Node &node = model.nodes[nodeindex];
		if(node.mesh > -1)
		{
			tinygltf::Mesh &tinymesh = model.meshes[node.mesh];
			cmfformat.MeshCount++;
			CMFFormat::Mesh cmfmesh;
			cmfmesh.NameSize = tinymesh.name.size() + 1;
			cmfmesh.Name = tinymesh.name.c_str();

			for(tinygltf::Primitive &primitive : tinymesh.primitives)
			{
				cmfmesh.VertexAttribCount = primitive.attributes.size() + (2 * metadata.CalculateTangentSpace);
				cmfmesh.VertexAttribs.resize(cmfmesh.VertexAttribCount);
				std::map<std::string, int>::const_iterator it(primitive.attributes.begin());
				std::map<std::string, int>::const_iterator itEnd(primitive.attributes.end());

				// ___ For Tangent Space Calculation ___
				uint32_t vertices_count = 0;
				float* position_array;
				float* tex_coord_array;
				// _____________________________________

				for(uint32_t i = 0; it != itEnd; it++, i++)
				{
					CMFFormat::VertexAttrib &cmfattrib = cmfmesh.VertexAttribs[i];

					tinygltf::Accessor &accessor = model.accessors[it->second];
					tinygltf::BufferView &bufferview = model.bufferViews[accessor.bufferView];
					tinygltf::Buffer &buffer = model.buffers[bufferview.buffer];

					if(it->first.compare("POSITION") == 0)
					{
						cmfattrib.NameSize = 9;
						cmfattrib.Name = "POSITION";
						cmfattrib.ShaderDataType = ShaderDataType::Float3;
						cmfattrib.VertexAttribType = VertexAttribType::Position;
						cmfattrib.BufferSize = bufferview.byteLength;
						cmfattrib.Buffer = buffer.data.data() + bufferview.byteOffset;
						position_array = (float*)cmfattrib.Buffer;
						vertices_count = bufferview.byteLength / (sizeof(float) * 3);
					}

					if(it->first.compare("NORMAL") == 0)
					{
						cmfattrib.NameSize = 7;
						cmfattrib.Name = "NORMAL";
						cmfattrib.ShaderDataType = ShaderDataType::Float3;
						cmfattrib.VertexAttribType = VertexAttribType::Normal;
						cmfattrib.BufferSize = bufferview.byteLength;
						cmfattrib.Buffer = buffer.data.data() + bufferview.byteOffset;
					}

					if(it->first.compare("TEXCOORD_0") == 0)
					{
						cmfattrib.NameSize = 11;
						cmfattrib.Name = "TEXCOORD_0";
						cmfattrib.ShaderDataType = ShaderDataType::Float2;
						cmfattrib.VertexAttribType = VertexAttribType::TexCoord;
						cmfattrib.BufferSize = bufferview.byteLength;
						s_LoadingData.modified_tex_coords = new float[vertices_count * 2];
						cmfattrib.Buffer = s_LoadingData.modified_tex_coords; // At this point, it is a nullptr. It will get filled afterwards.
						tex_coord_array = (float*)(buffer.data.data() + bufferview.byteOffset);
					}
				}

				// _________________________ Texture Coordinates Modification ____________________

				for(uint32_t i = 0; i < vertices_count; i++)
				{
					s_LoadingData.modified_tex_coords[i * 2    ] = tex_coord_array[i * 2];
					s_LoadingData.modified_tex_coords[i * 2 + 1] = 1.0f - tex_coord_array[i * 2 + 1];
				}

				// Now let tex_coord_array point to the modified array too
				tex_coord_array = s_LoadingData.modified_tex_coords;
				
				//__________________________________ Indices ___________________________________

				{   
					tinygltf::Accessor &accessor = model.accessors[primitive.indices];
					tinygltf::BufferView &bufferview = model.bufferViews[accessor.bufferView];
					tinygltf::Buffer &buffer = model.buffers[bufferview.buffer];
					cmfmesh.IndexBufferSize = bufferview.byteLength;
					cmfmesh.IndexBuffer = (uint16_t*)(buffer.data.data() + bufferview.byteOffset);
				}

				// ________________________ Tangent Space Calculation _____________________________

				if(metadata.CalculateTangentSpace)
				{	
					s_LoadingData.tangent_buffer = new float[vertices_count * 3];
					s_LoadingData.bitangent_buffer = new float[vertices_count * 3];

					for(uint32_t i = 0; i < vertices_count; i+=3)
					{
						uint16_t v1 = cmfmesh.IndexBuffer[i + 0];
						uint16_t v2 = cmfmesh.IndexBuffer[i + 1];
						uint16_t v3 = cmfmesh.IndexBuffer[i + 2];

						TangentVectors vectors = CalculateTangentVectors(
							{position_array[3 * (v2) + 0] - position_array[3 * (v1) + 0],
							 position_array[3 * (v2) + 1] - position_array[3 * (v1) + 1],
							 position_array[3 * (v2) + 2] - position_array[3 * (v1) + 2]},

							{position_array[3 * (v3) + 0] - position_array[3 * (v1) + 0],
							 position_array[3 * (v3) + 1] - position_array[3 * (v1) + 1],
							 position_array[3 * (v3) + 2] - position_array[3 * (v1) + 2]},

							{glm::fract(tex_coord_array[2 * (v2) + 0]) - glm::fract(tex_coord_array[2 * (v1) + 0]),
							 glm::fract(tex_coord_array[2 * (v2) + 1]) - glm::fract(tex_coord_array[2 * (v1) + 1])},

							{glm::fract(tex_coord_array[2 * (v3) + 0]) - glm::fract(tex_coord_array[2 * (v1) + 0]),
							 glm::fract(tex_coord_array[2 * (v3) + 1]) - glm::fract(tex_coord_array[2 * (v1) + 1])}
							);

						s_LoadingData.tangent_buffer[v1 * 3 + 0] = vectors.Tangent[0];
						s_LoadingData.tangent_buffer[v1 * 3 + 1] = vectors.Tangent[1];
						s_LoadingData.tangent_buffer[v1 * 3 + 2] = vectors.Tangent[2];
						s_LoadingData.tangent_buffer[v2 * 3 + 0] = vectors.Tangent[0];
						s_LoadingData.tangent_buffer[v2 * 3 + 1] = vectors.Tangent[1];
						s_LoadingData.tangent_buffer[v2 * 3 + 2] = vectors.Tangent[2];
						s_LoadingData.tangent_buffer[v3 * 3 + 0] = vectors.Tangent[0];
						s_LoadingData.tangent_buffer[v3 * 3 + 1] = vectors.Tangent[1];
						s_LoadingData.tangent_buffer[v3 * 3 + 2] = vectors.Tangent[2];

						s_LoadingData.bitangent_buffer[v1 * 3 + 0] = vectors.Bitangent[0];
						s_LoadingData.bitangent_buffer[v1 * 3 + 1] = vectors.Bitangent[1];
						s_LoadingData.bitangent_buffer[v1 * 3 + 2] = vectors.Bitangent[2];
						s_LoadingData.bitangent_buffer[v2 * 3 + 0] = vectors.Bitangent[0];
						s_LoadingData.bitangent_buffer[v2 * 3 + 1] = vectors.Bitangent[1];
						s_LoadingData.bitangent_buffer[v2 * 3 + 2] = vectors.Bitangent[2];
						s_LoadingData.bitangent_buffer[v3 * 3 + 0] = vectors.Bitangent[0];
						s_LoadingData.bitangent_buffer[v3 * 3 + 1] = vectors.Bitangent[1];
						s_LoadingData.bitangent_buffer[v3 * 3 + 2] = vectors.Bitangent[2];
					}

					{
						CMFFormat::VertexAttrib &tangent_attrib = cmfmesh.VertexAttribs[cmfmesh.VertexAttribCount - 2];
						tangent_attrib.NameSize = 8;
						tangent_attrib.Name = "TANGENT";
						tangent_attrib.ShaderDataType = ShaderDataType::Float3;
						tangent_attrib.VertexAttribType = VertexAttribType::Tangent;
						tangent_attrib.BufferSize = (vertices_count * 3) * sizeof(float);
						tangent_attrib.Buffer = s_LoadingData.tangent_buffer;
					}

					{
						CMFFormat::VertexAttrib &bitangent_attrib = cmfmesh.VertexAttribs[cmfmesh.VertexAttribCount - 1];
						bitangent_attrib.NameSize = 10;
						bitangent_attrib.Name = "BITANGENT";
						bitangent_attrib.ShaderDataType = ShaderDataType::Float3;
						bitangent_attrib.VertexAttribType = VertexAttribType::Bitangent;
						bitangent_attrib.BufferSize = (vertices_count * 3) * sizeof(float);
						bitangent_attrib.Buffer = s_LoadingData.bitangent_buffer;
					}

				}

				{   // Materials
					CMFFormat::Material cmfmaterial;
					tinygltf::Material &tinymaterial = model.materials[primitive.material];

					cmfmaterial.NameSize = tinymaterial.name.size() + 1;
					cmfmaterial.Name = tinymaterial.name.c_str();
					
					cmfmaterial.RoughnessFactor = tinymaterial.pbrMetallicRoughness.roughnessFactor;
					cmfmaterial.MetallicFactor = tinymaterial.pbrMetallicRoughness.metallicFactor;

					std::vector<double> &base = tinymaterial.pbrMetallicRoughness.baseColorFactor;
					cmfmaterial.BaseColour[0] = (float)base[0];
					cmfmaterial.BaseColour[1] = (float)base[1];
					cmfmaterial.BaseColour[2] = (float)base[2];
					cmfmaterial.BaseColour[3] = (float)base[3];

					// Textures
					{
						if(tinymaterial.pbrMetallicRoughness.baseColorTexture.index > -1)
						{
							std::string& path = model.images[model.textures[tinymaterial.pbrMetallicRoughness.baseColorTexture.index].source].uri;
							CMFFormat::Material::Texture texture = {
								(uint32_t)TextureType::Albedo,
								(uint32_t)path.size() + 1,
								path.c_str()
							};
							cmfmaterial.Textures.push_back(texture);
						}

						if(tinymaterial.pbrMetallicRoughness.metallicRoughnessTexture.index > -1)
						{
							std::string& path = model.images[model.textures[tinymaterial.pbrMetallicRoughness.metallicRoughnessTexture.index].source].uri;
							CMFFormat::Material::Texture texture = {
								(uint32_t)TextureType::Metallic_Roughness,
								(uint32_t)path.size() + 1,
								path.c_str()
							};
							cmfmaterial.Textures.push_back(texture);
						}

						if(tinymaterial.normalTexture.index > -1)
						{
							std::string& path = model.images[model.textures[tinymaterial.normalTexture.index].source].uri;
							CMFFormat::Material::Texture texture = {
								(uint32_t)TextureType::Normal,
								(uint32_t)path.size() + 1,
								path.c_str()
							};
							cmfmaterial.Textures.push_back(texture);
						}
						cmfmaterial.TextureCount = cmfmaterial.Textures.size();
					}

					cmfformat.Materials.push_back(cmfmaterial);

				}
			}

			cmfformat.MaterialCount = cmfformat.Materials.size();

			cmfformat.Meshes.push_back(cmfmesh);
		}

		cmfformat.MeshCount = cmfformat.Meshes.size();
	}
}

void Cleanup(ModelLoadingMetadata &metadata)
{
	if(metadata.CalculateTangentSpace)
	{
		free(s_LoadingData.tangent_buffer);
		free(s_LoadingData.bitangent_buffer);
		free(s_LoadingData.modified_tex_coords);
	}
}
