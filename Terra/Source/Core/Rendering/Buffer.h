#pragma once

namespace Terra
{
    class VertexBuffer
    {
    public:
        VertexBuffer(const void* data, unsigned int size);
        VertexBuffer(unsigned int size);
        ~VertexBuffer();
        void Bind() const;
        void Unbind() const;

        void SetData(unsigned int size, const void* data);
    private:
        unsigned int m_ID = 0;
    };

    class IndexBuffer
    {
    public:
        IndexBuffer(const unsigned int* data, unsigned int count);
        ~IndexBuffer();
        void Bind() const;
        void Unbind() const;

        unsigned int GetCount() const { return m_Count; }
    private:
        unsigned int m_ID = 0;
        unsigned int m_Count = 0;
    };
}
