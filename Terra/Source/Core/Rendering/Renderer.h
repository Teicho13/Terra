#pragma once
#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>
#include <mat4x4.hpp>
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"

namespace Terra
{

    struct RenderStats
    {
        static void ResetData() { s_DrawnQuads = 0; s_DrawCalls = 0; s_DrawnTextures = 0;}
        
        inline static unsigned int s_DrawnQuads = 0;
        inline static int s_DrawCalls = 0;
        inline static int s_DrawnTextures = 0;
    };

    struct TextureParameters
    {
        std::shared_ptr<Texture> Texture = nullptr;
        glm::vec2* textureCoords = nullptr;
        bool FlipX = false;
        bool FlipY = false;
    };

    struct QuadParameters
    {
        glm::mat4 Transform {1.f};
        glm::vec4 Color {1.f};
        TextureParameters TextureParams;
    };
    
    class Camera;
    class Renderer
    {
    public:
        static void Initialize();
        static void Shutdown();
        
        static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
        static void DrawQuad(const glm::mat4& transform, const std::shared_ptr<Texture>& texture, bool flipX = false, bool flipY = false);
        static void DrawQuad(const glm::mat4& transform, const std::shared_ptr<Texture>& texture,glm::vec2 textureCoords[4], bool flipX = false, bool flipY = false);
        
        
        static void DrawQuad(const glm::vec3& position,  const glm::vec3& size, const glm::vec4& color);
        static void DrawQuad(const glm::vec3& position,  const glm::vec3& size, const std::shared_ptr<Texture>& texture, bool flipX = false, bool flipY = false);
        static void DrawQuad(const glm::vec3& position,  const glm::vec3& size, const std::shared_ptr<Texture>& texture,glm::vec2 textureCoords[4], bool flipX = false, bool flipY = false);
        
        static void RenderScene(const Camera& camera);
        static void Flush();
        static void Clear();
        
    private:
        static void DrawQuadInternal(QuadParameters& parameters);
        static void FlushBatch();
        static void DrawIndices(VertexArray& va, GLsizei count);
    };   
}

