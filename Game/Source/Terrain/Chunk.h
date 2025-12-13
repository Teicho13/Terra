#pragma once
#include <vec2.hpp>
#include <vec3.hpp>
#include <array>


#include "TerrainGenerator.h"

class Chunk
{
public:
    Chunk(glm::vec2 position);
    
    glm::vec2 getPosition() const;

   std::array<std::array<glm::vec3,TerrainGenerator::CHUNK_HEIGHT>,TerrainGenerator::CHUNK_WIDTH> m_ChunkData;

private:
    glm::vec2 m_Position;
};
