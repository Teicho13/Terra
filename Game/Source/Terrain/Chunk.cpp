#include "Chunk.h"


Chunk::Chunk(const glm::vec2 position)
    : m_Position(position)
{
}

glm::vec2 Chunk::getPosition() const
{
    return m_Position;
}
