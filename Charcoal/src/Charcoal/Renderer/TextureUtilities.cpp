#include "chpch.h"
#include "TextureUtilities.h"

#include "Charcoal/Renderer/RendererAPI.h"

namespace Charcoal
{
namespace TextureUtilities
{

// ____________________________ Static Variables ______________________________

	Ref<VertexArray> s_CubeVertexArray;

// ____________________________ Function Declarations _________________________
	
	static void GenerateCubeVertexArray();

// _____________________________ Utility Functions ____________________________

	void InitTextureUtilities()
	{
		GenerateCubeVertexArray();
	}

// ____________________________ Static Functions ______________________________

	static void GenerateCubeVertexArray()
	{
		uint32_t indices[36] = {
			0, 2, 4,
			3, 7, 2,
			7, 5, 6,
			5, 7, 1,
			1, 3, 0,
			5, 1, 4,
			2, 6, 4,
			7, 6, 2,
			5, 4, 6,
			7, 3, 1,
			3, 2, 0,
			1, 0, 4
		};

		float vertices[8 * 3] = {
			0.5f,  0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f,  0.5f,  0.5f,
			0.5f, -0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,
		};
			
		s_CubeVertexArray = VertexArray::Create();
		s_CubeVertexArray->Bind();
		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(sizeof(vertices), vertices);
		BufferLayout layout ={
			{ ShaderDataType::Float3, "a_Pos" }
		};
		vertexBuffer->SetLayout(layout);
		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(DataType::UInt32, sizeof(indices), indices);
		s_CubeVertexArray->AddVertexBuffer(vertexBuffer);
		s_CubeVertexArray->SetIndexBuffer(indexBuffer);
		s_CubeVertexArray->Unbind();
	}
	
}    
}
