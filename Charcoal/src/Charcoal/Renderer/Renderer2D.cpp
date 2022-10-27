#include "chpch.h"
#include "Renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Charcoal
{

    Renderer2D::SceneData *Renderer2D::m_SceneData = new Renderer2D::SceneData();
    Renderer2D::Storage *Renderer2D::m_Storage = new Renderer2D::Storage();
    Renderer2D::Statistics *Renderer2D::m_Stats = new Renderer2D::Statistics();

    void Renderer2D::Init()
    {
        CH_PROFILE_FUNCTION();

        m_Storage->TextureShader = ShaderManager::LoadShader("assets/shaders/Texture.glsl", ShaderLanguage::GLSL);
		uint32_t white = 0xFFFFFFFF;
        m_Storage->WhiteTexture = Texture2D::Create(1, 1, &white);
        m_Storage->QuadVertexArray = VertexArray::Create();

        m_Storage->TextureShader->Bind();
        std::vector<int32_t> slots = std::vector<int32_t>(m_Storage->MaxTextureSlots);
        for (int32_t i = 0; i < m_Storage->MaxTextureSlots; i++)
            slots[i] = i;
        m_Storage->TextureShader->SetIntArray("u_TextureSlots", m_Storage->MaxTextureSlots, slots.data());

        BufferLayout layout = {
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float2, "a_TexCoord"},
            {ShaderDataType::Float, "a_TextureSlot"},
            {ShaderDataType::Float4, "a_Colour"},
            {ShaderDataType::Float, "a_TilingFactor"}};
        m_Storage->QuadVertexBuffer = VertexBuffer::Create(m_Storage->MaxVertices * layout.GetStride());
        m_Storage->QuadVertexBuffer->SetLayout(layout);
        m_Storage->QuadVertexArray->AddVertexBuffer(m_Storage->QuadVertexBuffer);

        uint32_t *indexData = new uint32_t[m_Storage->MaxIndices];
        uint32_t offset = 0;
        for (uint32_t i = 0; i < m_Storage->MaxIndices; i += 6)
        {
            indexData[i] = 0 + offset;
            indexData[i + 1] = 1 + offset;
            indexData[i + 2] = 2 + offset;
            indexData[i + 3] = 2 + offset;
            indexData[i + 4] = 3 + offset;
            indexData[i + 5] = 0 + offset;

            offset += 4;
        }
        m_Storage->QuadIndexBuffer = IndexBuffer::Create(DataType::UInt32, m_Storage->MaxIndices * sizeof(uint32_t), indexData);
        m_Storage->QuadVertexArray->SetIndexBuffer(m_Storage->QuadIndexBuffer);

        m_Storage->QuadVerticesBase = new Storage::QuadVertex[m_Storage->MaxVertices];
        m_Storage->QuadVerticesPtr = m_Storage->QuadVerticesBase;

        m_Storage->TextureSlots.resize(m_Storage->MaxTextureSlots); // TODO: Query from Renderer Capabilities
        m_Storage->TextureSlots[0] = m_Storage->WhiteTexture;
        m_Storage->ReservedTexSlotsCount++;
        m_Storage->OccupiedTexSlotCount = m_Storage->ReservedTexSlotsCount;
    }

    void Renderer2D::ShutDown()
    {
        delete m_SceneData;
        delete m_Storage;
        delete m_Stats;
    }

    void Renderer2D::BeginScene(const OrthographicCamera &camera)
    {
        CH_PROFILE_FUNCTION();

        m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
        m_Stats->MaxQuads = m_Storage->MaxQuads;
    }

    void Renderer2D::EndScene()
    {
        CH_PROFILE_FUNCTION();

        if (m_Storage->QuadCount > 0)
            Flush();
    }

    void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &scale, const glm::vec4 &colour)
    {
        DrawQuad(position, scale, m_Storage->WhiteTexture, colour);
    }

    void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &scale, Ref<Texture2D> texture, const glm::vec4 &tint)
    {
        CH_PROFILE_FUNCTION();

        if (m_Storage->QuadCount == m_Storage->MaxQuads)
            Flush();

        uint32_t textureSlot = 0;
        bool textureFound = false;
        for (uint32_t i = m_Storage->ReservedTexSlotsCount; i < m_Storage->OccupiedTexSlotCount; i++)
        {
            if (m_Storage->TextureSlots[i] == texture)
            {
                textureSlot = i;
                textureFound = true;
                break;
            }
        }
        if (!textureFound)
        {
            if (m_Storage->OccupiedTexSlotCount > (int32_t)m_Storage->MaxTextureSlots)
            {
                Flush();
            }
            m_Storage->TextureSlots[m_Storage->OccupiedTexSlotCount] = texture;
            textureSlot = m_Storage->OccupiedTexSlotCount;
            m_Storage->OccupiedTexSlotCount++;
        }

        glm::mat4 transform = m_SceneData->ViewProjectionMatrix * glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(scale, 1.0f));
        glm::vec4 tempPos;

        tempPos = transform * glm::vec4(-0.5, -0.5f, 0, 1.0f);
        m_Storage->QuadVerticesPtr->Position[0] = tempPos.x;
        m_Storage->QuadVerticesPtr->Position[1] = tempPos.y;
        m_Storage->QuadVerticesPtr->Position[2] = tempPos.z;
        m_Storage->QuadVerticesPtr->TexCoord[0] = 0.0f;
        m_Storage->QuadVerticesPtr->TexCoord[1] = 0.0f;
        m_Storage->QuadVerticesPtr->TextureSlot = textureSlot;
        m_Storage->QuadVerticesPtr->Colour[0] = tint.r;
        m_Storage->QuadVerticesPtr->Colour[1] = tint.g;
        m_Storage->QuadVerticesPtr->Colour[2] = tint.b;
        m_Storage->QuadVerticesPtr->Colour[3] = tint.a;
        m_Storage->QuadVerticesPtr->TilingFactor = 1.0f;
        m_Storage->QuadVerticesPtr++;

        tempPos = transform * glm::vec4(0.5, -0.5f, 0, 1.0f);
        m_Storage->QuadVerticesPtr->Position[0] = tempPos.x;
        m_Storage->QuadVerticesPtr->Position[1] = tempPos.y;
        m_Storage->QuadVerticesPtr->Position[2] = tempPos.z;
        m_Storage->QuadVerticesPtr->TexCoord[0] = 1.0f;
        m_Storage->QuadVerticesPtr->TexCoord[1] = 0.0f;
        m_Storage->QuadVerticesPtr->TextureSlot = textureSlot;
        m_Storage->QuadVerticesPtr->Colour[0] = tint.r;
        m_Storage->QuadVerticesPtr->Colour[1] = tint.g;
        m_Storage->QuadVerticesPtr->Colour[2] = tint.b;
        m_Storage->QuadVerticesPtr->Colour[3] = tint.a;
        m_Storage->QuadVerticesPtr->TilingFactor = 1.0f;
        m_Storage->QuadVerticesPtr++;

        tempPos = transform * glm::vec4(0.5, 0.5f, 0, 1.0f);
        m_Storage->QuadVerticesPtr->Position[0] = tempPos.x;
        m_Storage->QuadVerticesPtr->Position[1] = tempPos.y;
        m_Storage->QuadVerticesPtr->Position[2] = tempPos.z;
        m_Storage->QuadVerticesPtr->TexCoord[0] = 1.0f;
        m_Storage->QuadVerticesPtr->TexCoord[1] = 1.0f;
        m_Storage->QuadVerticesPtr->TextureSlot = textureSlot;
        m_Storage->QuadVerticesPtr->Colour[0] = tint.r;
        m_Storage->QuadVerticesPtr->Colour[1] = tint.g;
        m_Storage->QuadVerticesPtr->Colour[2] = tint.b;
        m_Storage->QuadVerticesPtr->Colour[3] = tint.a;
        m_Storage->QuadVerticesPtr->TilingFactor = 1.0f;
        m_Storage->QuadVerticesPtr++;

        tempPos = transform * glm::vec4(-0.5, 0.5f, 0, 1.0f);
        m_Storage->QuadVerticesPtr->Position[0] = tempPos.x;
        m_Storage->QuadVerticesPtr->Position[1] = tempPos.y;
        m_Storage->QuadVerticesPtr->Position[2] = tempPos.z;
        m_Storage->QuadVerticesPtr->TexCoord[0] = 0.0f;
        m_Storage->QuadVerticesPtr->TexCoord[1] = 1.0f;
        m_Storage->QuadVerticesPtr->TextureSlot = textureSlot;
        m_Storage->QuadVerticesPtr->Colour[0] = tint.r;
        m_Storage->QuadVerticesPtr->Colour[1] = tint.g;
        m_Storage->QuadVerticesPtr->Colour[2] = tint.b;
        m_Storage->QuadVerticesPtr->Colour[3] = tint.a;
        m_Storage->QuadVerticesPtr->TilingFactor = 1.0f;
        m_Storage->QuadVerticesPtr++;

        m_Storage->QuadCount++;
        m_Stats->QuadCount++;
    }

    void Renderer2D::ResetStats()
    {
        m_Stats->QuadCount = 0;
        m_Stats->DrawCalls = 0;
    }

    void Renderer2D::Flush()
    {
        CH_PROFILE_FUNCTION();
        
        m_Storage->QuadVertexBuffer->SetData((float *)m_Storage->QuadVerticesBase, m_Storage->QuadVertexBuffer->GetLayout().GetStride() * m_Storage->QuadCount * 4);

        m_Storage->TextureShader->Bind();
        for (int32_t i = 0; i < m_Storage->OccupiedTexSlotCount; i++)
        {
            m_Storage->TextureSlots[i]->Bind(i);
        }
        m_Storage->QuadVertexArray->Bind();
        RendererCommand::DrawIndexed(m_Storage->QuadVertexArray, m_Storage->QuadCount * 6);

        m_Stats->DrawCalls++;

        m_Storage->QuadCount = 0;
        m_Storage->OccupiedTexSlotCount = m_Storage->ReservedTexSlotsCount;
        m_Storage->QuadVerticesPtr = m_Storage->QuadVerticesBase;
    }

} // namespace Charcoal
