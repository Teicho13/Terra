#include "Buffer.h"

#include <glad/glad.h>

Terra::VertexBuffer::VertexBuffer(const void* data, const unsigned int size)
{
    glGenBuffers(1, &m_ID);
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

Terra::VertexBuffer::VertexBuffer(const unsigned int size)
{
    glGenBuffers(1, &m_ID);
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

Terra::VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_ID);
}

void Terra::VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
}

void Terra::VertexBuffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//Index Buffer

Terra::IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
    : m_Count(count)
{
    glGenBuffers(1, &m_ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

Terra::IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &m_ID);
}

void Terra::IndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
}

void Terra::IndexBuffer::Unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
