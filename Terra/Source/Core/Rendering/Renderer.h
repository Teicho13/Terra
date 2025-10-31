#pragma once
#include <fwd.hpp>
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"

namespace Terra
{
    class Camera;
    class Renderer
    {
    public:
        static void Initialize();
        static void Shutdown();
        
        static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
        static void DrawQuad(const glm::mat4& transform, const Texture& texture);
        
        static void DrawQuad(const glm::vec3& position,  const glm::vec3& size, const glm::vec4& color);
        static void DrawQuad(const glm::vec3& position,  const glm::vec3& size, const Texture& texture);
        
        static void RenderScene(const Camera& camera);
        static void Flush();
        static void Clear();
        
        inline static int s_TileSize = 10;
        inline static unsigned int s_DrawnQuads = 0;
        inline static int s_DrawCalls = 0;

    private:
        static void DrawIndices(VertexArray& va, GLsizei count);
    };   
}

