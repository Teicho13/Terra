#pragma once
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"

namespace Terra
{
    class Renderer
    {
    public:
        static void Draw(VertexArray& va, const Texture& tx, unsigned int shaderID);
        static void Draw(VertexArray& va, const Texture& tx, unsigned int shaderID, int amount);
        static void Clear();
    };   
}

