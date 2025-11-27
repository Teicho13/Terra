#pragma once
#include <array>
#include <vec2.hpp>
#include "Core/Rendering/Texture.h"

class ChunkManager;
class Chunk
{
public:
    Chunk(glm::vec2 position, ChunkManager* chunkManager, int test);

    void Render(std::shared_ptr<Terra::Texture> texture);

    bool TileIsWithinBounds(float x, float y) const;
    glm::vec2 getPosition() const;

    std::array<glm::vec2,4> GetTextureCoordinates(int tileID) const;

    //Max amount of tiles (width) inside a single chunk
    static constexpr int MAX_TILES_WIDTH = 16;
    //Max amount of tiles (height) inside a single chunk
    static constexpr int MAX_TILES_HEIGHT = 32;

private:
    void FillChunk(int test);
    
    std::array<std::array<int, MAX_TILES_WIDTH>,MAX_TILES_HEIGHT> m_ChunkData;
    glm::vec2 m_Position {0.f,0.f};
    ChunkManager* m_ChunkManagerRef = nullptr;
};
