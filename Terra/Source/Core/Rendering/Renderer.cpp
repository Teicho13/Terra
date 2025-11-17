#include "Renderer.h"
#include <array>
#include <ext/matrix_transform.hpp>

#include "Camera.h"
#include "VertexArray.h"
#include "Core/FileIO.h"

namespace Terra
{
    struct QuadVertexData
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 Texcoord;
        float TextureID;
        float ShouldUseTexture;
    };

    struct RendererData
    {
        //All storage for the renderer.
        std::unique_ptr<VertexArray> VertexArray;
        std::unique_ptr<VertexBuffer> VertexBuffer;
        std::unique_ptr<IndexBuffer> IndexBuffer;
        std::unique_ptr<Shader> QuadShader;
        
        QuadVertexData* QuadVertexDataAdress = nullptr;
        QuadVertexData* QuadVertexDataAdressCopy = nullptr;

        //Count for the amount of indices currently done.
        unsigned int QuadIndexCount = 0;
        
        //Max number of quads per draw call
        const unsigned int MaxQuads =       10000;
        const unsigned int MaxVertices =    MaxQuads * 4;
        const unsigned int MaxIndices =     MaxQuads * 6;
        
        //Default for pc is 32 slots
        static constexpr unsigned int MaxTextureSlots = 32;
        std::array<std::shared_ptr<Texture>, MaxTextureSlots> TexturesSlots;
        unsigned int CurrentTextureSlots = 0;
        
        glm::vec4 QuadVertexPositions[4];
        
    };

    static RendererData s_RendererData;

    void Renderer::Initialize()
    {
        //Create Vertex array and vertexBuffer that the renderer will use.
        s_RendererData.VertexArray = std::make_unique<VertexArray>();
        s_RendererData.VertexBuffer = std::make_unique<VertexBuffer>(s_RendererData.MaxVertices * sizeof(QuadVertexData));
    
        //Link all VBO attributes.
        s_RendererData.VertexArray->AddAttribute(*s_RendererData.VertexBuffer,{.count = 3, .type = GL_FLOAT, .normalized = GL_FALSE, .stride = sizeof(QuadVertexData), .offset = (void*)offsetof(QuadVertexData, Position)});
        s_RendererData.VertexArray->AddAttribute(*s_RendererData.VertexBuffer,{.count = 4, .type = GL_FLOAT, .normalized = GL_FALSE, .stride = sizeof(QuadVertexData), .offset = (void*)offsetof(QuadVertexData, Color)});
        s_RendererData.VertexArray->AddAttribute(*s_RendererData.VertexBuffer,{.count = 2, .type = GL_FLOAT, .normalized = GL_FALSE, .stride = sizeof(QuadVertexData), .offset = (void*)offsetof(QuadVertexData, Texcoord)});
        s_RendererData.VertexArray->AddAttribute(*s_RendererData.VertexBuffer,{.count = 1, .type = GL_FLOAT  , .normalized = GL_FALSE, .stride = sizeof(QuadVertexData), .offset = (void*)offsetof(QuadVertexData, TextureID)});
        s_RendererData.VertexArray->AddAttribute(*s_RendererData.VertexBuffer,{.count = 1, .type = GL_FLOAT, .normalized = GL_FALSE, .stride = sizeof(QuadVertexData), .offset = (void*)offsetof(QuadVertexData, ShouldUseTexture)});
    
        //Set pointer to start of the array of quad vertices.
        s_RendererData.QuadVertexDataAdress = new QuadVertexData[s_RendererData.MaxVertices];
    
        //Generate indices (in the order 0,2,1,0,3,2) based on the amount of quads we want.
        
        //Heap allocation to prevent stack overflow.
        auto* indices = new unsigned int[s_RendererData.MaxIndices];
        unsigned int offset = 0;
        for (unsigned int i = 0; i < s_RendererData.MaxIndices; i += 6)
        {
            indices[i + 0] = 0 + offset;
            indices[i + 1] = 2 + offset;
            indices[i + 2] = 1 + offset;
            
            indices[i + 3] = 0 + offset;
            indices[i + 4] = 3 + offset;
            indices[i + 5] = 2 + offset;
    
            offset += 4;
        }
    
        //Create index buffer and bind generated indices.
        s_RendererData.IndexBuffer = std::make_unique<IndexBuffer>(indices, s_RendererData.MaxIndices);
        delete[] indices;
    
        //Create Shader
        s_RendererData.QuadShader = std::make_unique<Shader>(FileIO::GetEngineFile("Shaders\\DefaultVertex.vert"),FileIO::GetEngineFile("Shaders\\DefaultFragment.frag"));

        //Default size of the quad.

        s_RendererData.QuadVertexPositions[0] = {  0.f,  0.f, 0.0f, 1.0f };
        s_RendererData.QuadVertexPositions[1] = {  0.f,  1.f, 0.0f, 1.0f };
        s_RendererData.QuadVertexPositions[2] = {  1.f,  1.f, 0.0f, 1.0f };
        s_RendererData.QuadVertexPositions[3] = {  1.f,  0.f, 0.0f, 1.0f };
    }

    void Renderer::Shutdown()
    {
        delete[] s_RendererData.QuadVertexDataAdress;
    }

    void Renderer::RenderScene(const Camera& camera)
    {
        //Bind our shader
        s_RendererData.QuadShader->Bind();
    
        //Set vertex uniform values
        s_RendererData.QuadShader->SetMat4("u_ProjectionViewMatrix", camera.GetProjectionViewMatrix());

        //Reset data for new frame.
        s_RendererData.QuadIndexCount = 0;
        s_RendererData.QuadVertexDataAdressCopy = s_RendererData.QuadVertexDataAdress;
        s_RendererData.CurrentTextureSlots = 0;
        
        s_DrawnQuads = 0;
        s_DrawCalls = 0;
    }

    void Renderer::FlushBatch()
    {
        Flush();
            
        //Reset our count and set Ptr back to the start.
        s_RendererData.QuadIndexCount = 0;
        s_RendererData.QuadVertexDataAdressCopy = s_RendererData.QuadVertexDataAdress;
        s_RendererData.CurrentTextureSlots = 0;
    }

    void Renderer::Clear()
    {
        //Clear screen
        glClearColor(0.f, 0.1f, 0.2f, 255.f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    
    void Renderer::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
    {
        //We now the quad vertices and texture coords ahead of time.
        constexpr size_t quadVertexCount = 4;
        constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f } };


        //Check if we need to flush the already added quads before adding the new one
        if (s_RendererData.QuadIndexCount >= s_RendererData.MaxIndices)
        {
            FlushBatch();
        }
        
        //Set data for each vertex
        for (size_t i = 0; i < quadVertexCount; i++)
        {
            s_RendererData.QuadVertexDataAdressCopy->Position = transform * s_RendererData.QuadVertexPositions[i];
            s_RendererData.QuadVertexDataAdressCopy->Color = color;
            s_RendererData.QuadVertexDataAdressCopy->Texcoord = textureCoords[i];
            s_RendererData.QuadVertexDataAdressCopy->TextureID = 0.f;
            s_RendererData.QuadVertexDataAdressCopy->ShouldUseTexture = 0.f;
            s_RendererData.QuadVertexDataAdressCopy++;
        }
    
        s_RendererData.QuadIndexCount += 6;
        s_DrawnQuads++;
    }
    
    void Renderer::DrawQuad(const glm::mat4& transform, const std::shared_ptr<Texture>& texture)
    {
        //We now the quad vertices and texture coords ahead of time.
        constexpr size_t quadVertexCount = 4;
        constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f } };

        //Check if we need to flush the already added quads before adding the new one
        if (s_RendererData.QuadIndexCount >= s_RendererData.MaxIndices)
        {
            FlushBatch();
        }

        float textureIndex = -1.0f;
        //Loop over our previously set texture slots to see if we already have the texture, if so set the index.

        for (unsigned int i = 0; i < s_RendererData.CurrentTextureSlots; i++)
        {
            if (s_RendererData.TexturesSlots[i]->GetID() == texture->GetID())
            {
                textureIndex = static_cast<float>(i);
                break;
            }
        }
        

        if (textureIndex == -1.0f)
        {
            //If we reached the max amount of textures per draw call flush early.
            if (s_RendererData.CurrentTextureSlots >= s_RendererData.MaxTextureSlots)
            {
                FlushBatch();
            }

            textureIndex = static_cast<float>(s_RendererData.CurrentTextureSlots);
            s_RendererData.TexturesSlots[s_RendererData.CurrentTextureSlots] = texture;
            s_RendererData.CurrentTextureSlots++;
        }

        //Set data for each vertex
        for (size_t i = 0; i < quadVertexCount; i++)
        {
            s_RendererData.QuadVertexDataAdressCopy->Position = transform * s_RendererData.QuadVertexPositions[i];
            s_RendererData.QuadVertexDataAdressCopy->Color = glm::vec4(1.f);
            s_RendererData.QuadVertexDataAdressCopy->Texcoord = textureCoords[i];
            s_RendererData.QuadVertexDataAdressCopy->TextureID = textureIndex;
            s_RendererData.QuadVertexDataAdressCopy->ShouldUseTexture = 1.f;
            s_RendererData.QuadVertexDataAdressCopy++;
        }
    
        s_RendererData.QuadIndexCount += 6;
        s_DrawnQuads++;
    }

    void Renderer::DrawQuad(const glm::mat4& transform, const std::shared_ptr<Texture>& texture,
        glm::vec2 textureCoords[4])
    {
        //We now the quad vertices and texture coords ahead of time.
        constexpr size_t quadVertexCount = 4;
        
        //Check if we need to flush the already added quads before adding the new one
        if (s_RendererData.QuadIndexCount >= s_RendererData.MaxIndices)
        {
            FlushBatch();
        }

        float textureIndex = -1.0f;
        //Loop over our previously set texture slots to see if we already have the texture, if so set the index.

        for (unsigned int i = 0; i < s_RendererData.CurrentTextureSlots; i++)
        {
            if (s_RendererData.TexturesSlots[i]->GetID() == texture->GetID())
            {
                textureIndex = static_cast<float>(i);
                break;
            }
        }
        

        if (textureIndex == -1.0f)
        {
            //If we reached the max amount of textures per draw call flush early.
            if (s_RendererData.CurrentTextureSlots >= s_RendererData.MaxTextureSlots)
            {
                FlushBatch();
            }

            textureIndex = static_cast<float>(s_RendererData.CurrentTextureSlots);
            s_RendererData.TexturesSlots[s_RendererData.CurrentTextureSlots] = texture;
            s_RendererData.CurrentTextureSlots++;
        }

        //Set data for each vertex
        for (size_t i = 0; i < quadVertexCount; i++)
        {
            s_RendererData.QuadVertexDataAdressCopy->Position = transform * s_RendererData.QuadVertexPositions[i];
            s_RendererData.QuadVertexDataAdressCopy->Color = glm::vec4(1.f);
            s_RendererData.QuadVertexDataAdressCopy->Texcoord = textureCoords[i];
            s_RendererData.QuadVertexDataAdressCopy->TextureID = textureIndex;
            s_RendererData.QuadVertexDataAdressCopy->ShouldUseTexture = 1.f;
            s_RendererData.QuadVertexDataAdressCopy++;
        }
    
        s_RendererData.QuadIndexCount += 6;
        s_DrawnQuads++;
    }

    void Renderer::DrawQuad(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        DrawQuad(transform, color);
    }

    void Renderer::DrawQuad(const glm::vec3& position, const glm::vec3& size, const std::shared_ptr<Texture>& texture)
    {
        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        DrawQuad(transform, texture);
    }

    void Renderer::DrawQuad(const glm::vec3& position, const glm::vec3& size, const std::shared_ptr<Texture>& texture,
        glm::vec2 textureCoords[4])
    {
        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        DrawQuad(transform, texture, textureCoords);
    }
    
    void Renderer::Flush()
    {
        //Check if there are indices to draw
        if (s_RendererData.QuadIndexCount > 0)
        {
            //We cast to uint8_t because it is 1 byte and by subtracting one from the other we know what the size of our data is in bytes.
    
            unsigned int dataSize = (uint32_t)((uint8_t*)s_RendererData.QuadVertexDataAdressCopy - (uint8_t*)s_RendererData.QuadVertexDataAdress);
            s_RendererData.VertexBuffer->SetData(dataSize,s_RendererData.QuadVertexDataAdress);

            //Get the location of our Textures array from the fragment shader and loop over it based on the amount of textures to set the correct index values.
            GLuint textureArrayLocation = s_RendererData.QuadShader->GetUniformLocation("u_Textures");
            for (int i = 0; i < s_RendererData.CurrentTextureSlots; i++)
            {
                s_RendererData.QuadShader->SetInt(textureArrayLocation + i, i);
                s_RendererData.TexturesSlots[i]->Bind(i);
            }
            
            DrawIndices(*s_RendererData.VertexArray,static_cast<GLint>(s_RendererData.QuadIndexCount));
            s_DrawCalls++;
        }
    }

    void Renderer::DrawIndices(VertexArray& va, const GLsizei count)
    {
        va.Bind();
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
    }
}


