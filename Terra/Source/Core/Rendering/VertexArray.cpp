#include "VertexArray.h"
#include <glad/glad.h>
#include "Buffer.h"


namespace Terra
{
    VertexArray::VertexArray()
    {
        glGenVertexArrays(1, &m_ID);
    }

    VertexArray::~VertexArray()
    {
        m_VertexAttributes.clear();
        glDeleteVertexArrays(1, &m_ID);
    }

    void VertexArray::Bind()
    {
        glBindVertexArray(m_ID);
    }

    void VertexArray::Unbind()
    {
        glBindVertexArray(0);
    }

    void VertexArray::AddAttribute(const VertexBuffer& vertexBuffer, const VertexAttribute& vertexAttrbute)
    {
        Bind();
        vertexBuffer.Bind();
        unsigned int NextIndex = static_cast<unsigned int>(m_VertexAttributes.size());
        const auto& va = vertexAttrbute;
        
        glEnableVertexAttribArray(NextIndex);
        glVertexAttribPointer(NextIndex, va.count, va.type, va.normalized, va.stride, va.offset);

        m_VertexAttributes.push_back(vertexAttrbute);
    }
}
