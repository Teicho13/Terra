#include "Chunk.h"
#include "ChunkManager.h"
#include "Core/Rendering/Renderer.h"
#include <vec3.hpp>

#include "Core/Application.h"
#include "Core/Rendering/Camera.h"
#include "PerlinNoise.hpp"

Chunk::Chunk(const glm::vec2 position, ChunkManager* chunkManager, int test)
    : m_Position(position), m_ChunkManagerRef(chunkManager)
{
    FillChunk(test);
}

void Chunk::Render(std::shared_ptr<Terra::Texture> texture)
{
    for (int y = 0; y < MAX_TILES_HEIGHT; ++y)
    {
        for (int x = 0; x < MAX_TILES_WIDTH; ++x)
        {
            if (TileIsWithinBounds(m_Position.x + x * 16.f,m_Position.y + y * 16.f) && m_ChunkData[y][x] != 0)
            {
                auto coords = GetTextureCoordinates(m_ChunkData[y][x]);
                Terra::Renderer::DrawQuad({m_Position.x + x * 16.f, m_Position.y + y * 16.f,0.f},{16.f,16.f,1.f},texture,coords.data());
            }
        }
    }
}

bool Chunk::TileIsWithinBounds(float x, float y) const
{
    return ((x >= 0 + m_ChunkManagerRef->GetCameraRef()->GetPosition().x || x + 16.f >= 0 + m_ChunkManagerRef->GetCameraRef()->GetPosition().x) &&
               (x <= Terra::Application::GetApplication()->GetWindowBuffer().x + m_ChunkManagerRef->GetCameraRef()->GetPosition().x) &&
               (y >= 0 + m_ChunkManagerRef->GetCameraRef()->GetPosition().y || y + 16.f >= 0 + m_ChunkManagerRef->GetCameraRef()->GetPosition().y) &&
               (y <= Terra::Application::GetApplication()->GetWindowBuffer().y + m_ChunkManagerRef->GetCameraRef()->GetPosition().y));
}

glm::vec2 Chunk::getPosition() const
{
    return m_Position;
}

std::array<glm::vec2, 4> Chunk::GetTextureCoordinates(int tileID) const
{
    //we get the total amount of tiles in the textures.
    const int TextureTileCountX = (m_ChunkManagerRef->GetTextureRef()->GetWidth() / 16);
    const int TextureTileCountY = (m_ChunkManagerRef->GetTextureRef()->GetHeight() / 16);

    //We first get the target tile position.
    //We flip the TargetTileY because openGL starts from the bottom.

    const int TargetTileY = (TextureTileCountY - 1) - (tileID / TextureTileCountX);
    const int TargetTileX = tileID % TextureTileCountX;

    //we calculate the gap in between each frame based on the amount of tiles in each row and column.
    
    const float OffsetX = 1.f / static_cast<float>(TextureTileCountX);
    const float OffsetY = 1.f / static_cast<float>(TextureTileCountY);

    std::array<glm::vec2,4> TextureCoord;
    TextureCoord[0] = { (static_cast<float>(TargetTileX) * OffsetX), (static_cast<float>(TargetTileY) * OffsetY) };
    TextureCoord[1] = { (static_cast<float>(TargetTileX) * OffsetX), (static_cast<float>(TargetTileY) * OffsetY) + OffsetY };
    TextureCoord[2] = { (static_cast<float>(TargetTileX) * OffsetX) + OffsetX, (static_cast<float>(TargetTileY) * OffsetY) + OffsetY };
    TextureCoord[3] = { (static_cast<float>(TargetTileX) * OffsetX) + OffsetX, (static_cast<float>(TargetTileY) * OffsetY) };
    
    return TextureCoord;
}

void Chunk::FillChunk(int test)
{
    const siv::PerlinNoise::seed_type seed = 123456u * test;
    
    const siv::PerlinNoise perlin{ seed };
    
    for (int x = 0; x < MAX_TILES_WIDTH; ++x)
    {
        const double noise = perlin.octave1D_01((x * 0.01), 8);
        int groundLevel = (int)((noise + 1) * (MAX_TILES_HEIGHT / 2));
    
        for (int y = 0; y < MAX_TILES_HEIGHT; ++y)
        {
            if (y < groundLevel - 5) {
                // Underground tiles: stone or dirt
                m_ChunkData[y][x] = 3; // Stone
            } else if (y < groundLevel) {
                // Ground tiles: dirt
                m_ChunkData[y][x] = 2; // Dirt
            } else {
                // Above ground tiles: air
                m_ChunkData[y][x] = 0; // Air
            }
        }
    }
}
