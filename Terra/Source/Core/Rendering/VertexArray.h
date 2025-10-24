#pragma once
#include <vector>

#include "Buffer.h"

namespace Terra
{
    struct VertexAttribute
    {
        //Amount of values
        int count;
        //The value type eg: GL_Float
        unsigned int type;
        //Are the values normalized ?
        unsigned char normalized;
        //How many values are there before the next set begins
        int stride;
        //Offset from starting point of the correct values
        void* offset;
    };
    
    class VertexArray
    {
    public:

        VertexArray();
        ~VertexArray();
        void Bind();
        void Unbind();

        void AddAttribute(const VertexBuffer& vertexBuffer, const VertexAttribute& vertexAttrbute);
        
    private:
        unsigned int m_ID;
        std::vector<VertexAttribute> m_VertexAttributes;
    }; 
}

