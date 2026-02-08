#pragma once
#include <vec2.hpp>
#include <array>


#include "TerrainGenerator.h"

class Chunk
{
public:
    Chunk(glm::vec2 position,TerrainGenerator* TGRef, int id);

    void Update(float deltaTime);
    void Render(const std::shared_ptr<Terra::Texture>& texture) const;
    
    std::array<glm::vec2,4> GetTextureCoordinates(const Terra::Texture* textureRef, int tileID) const;

    bool IsTileInView(float x, float y) const;
    glm::vec2 getPosition() const;

   std::array<std::array<TileData,TerrainGenerator::CHUNK_HEIGHT>,TerrainGenerator::CHUNK_WIDTH> m_ChunkData {};

private:
    TerrainGenerator* m_TerrainGeneratorRef = nullptr;
    glm::vec2 m_Position;
    int m_TileID = 0;
};
