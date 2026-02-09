#include "Chunk.h"

#include "Core/Application.h"
#include "Core/ResourceManager.h"
#include "Core/Rendering/Camera.h"
#include "Core/Rendering/Renderer.h"

Chunk::Chunk(glm::vec2 position, TerrainGenerator* TGRef, const int id)
    :m_TerrainGeneratorRef(TGRef), m_Position(position), m_ChunkID(id)
{
    
}

static constexpr TileBitmaskDirection g_TileBitmaskDirections[] =
{
    {.offsetX = 0, .offsetY = 1, .bitmask = 1},
    {.offsetX = -1, .offsetY = 0, .bitmask = 2},
    {.offsetX = 1, .offsetY = 0, .bitmask = 4},
    {.offsetX = 0, .offsetY = -1, .bitmask = 8}
};

void Chunk::Update(float deltaTime)
{
    constexpr float tilesize = static_cast<float>(TerrainGenerator::TILE_SIZE);
    
    for (int x = 0; x < TerrainGenerator::CHUNK_WIDTH; ++x)
    {
        for (int y = 0; y < TerrainGenerator::CHUNK_HEIGHT; ++y)
        {
            auto& tile = m_ChunkData[x][y];

            //Check if current tile is not air.
            if (tile.tiletype == TileType::Air) continue;
            //Check if current tile is within camera view.
            if (!IsTileInView(m_Position.x + static_cast<float>(x) * tilesize, m_Position.y + static_cast<float>(y) * tilesize)) continue;

            int mask = 0;

            //Loop over all directions and add up the mask for each valid neighbor.
            for (const auto& direction : g_TileBitmaskDirections)
            {
                if (IsTileValidNeighbor(x,y,direction))
                {
                    mask |= direction.bitmask;
                }
            }

            tile.tilemask = mask;
        }
    }
}

void Chunk::Render(const std::shared_ptr<Terra::Texture>& texture) const
{
    float tilesize = static_cast<float>(TerrainGenerator::TILE_SIZE);
    for (int x = 0; x < TerrainGenerator::CHUNK_WIDTH; ++x)
    {
        for (int y = 0; y < TerrainGenerator::CHUNK_HEIGHT; ++y)
        {
            //Check if the tile is within camera view and if not air (since we dont want to draw for air)
            if (IsTileInView(m_Position.x + static_cast<float>(x) * tilesize, m_Position.y + static_cast<float>(y) * tilesize) && m_ChunkData[x][y].tiletype != TileType::Air)
            {
                //Get the correct texture coordinate from the texture atlas.

                auto TextureCoords = GetTextureCoordinates(texture.get(),static_cast<int>(m_ChunkData[x][y].tiletype));
                
                auto temp = Terra::ResourceManager::GetInstance().GetTexture(GetTileTypePath(m_ChunkData[x][y].tiletype));
                if (m_ChunkData[x][y].tiletype != TileType::Gold && m_ChunkData[x][y].tiletype != TileType::Diamond && m_ChunkData[x][y].tiletype != TileType::Copper)
                {
                    TextureCoords = GetTextureCoordinates(temp.get(),m_ChunkData[x][y].tilemask); 
                }
                
                Terra::Renderer::DrawQuad({m_Position.x + static_cast<float>(x) * tilesize, m_Position.y + static_cast<float>(y) * tilesize,0.f},{tilesize,tilesize,1.f},temp,TextureCoords.data());
            }
        }
    }
}

std::array<glm::vec2, 4> Chunk::GetTextureCoordinates(const Terra::Texture* textureRef, int tileID) const
{
    //we get the total amount of tiles in the textures.
    
    const int TextureTileCountX = (textureRef->GetWidth() / TerrainGenerator::TILE_SIZE);
    const int TextureTileCountY = (textureRef->GetHeight() / TerrainGenerator::TILE_SIZE);

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

bool Chunk::IsTileInView(float x, float y) const
{
    //If there is no active camera we just render regardless.
    if (!m_TerrainGeneratorRef->GetCameraRef()) return true;

    const auto cameraPos = m_TerrainGeneratorRef->GetCameraRef()->GetPosition();
    return ((x >= 0 + cameraPos.x || x + static_cast<float>(TerrainGenerator::TILE_SIZE) >= 0 + cameraPos.x) &&
               (x <= Terra::Application::GetApplication()->GetWindowBuffer().x * m_TerrainGeneratorRef->GetCameraRef()->GetZoom() + cameraPos.x) &&
               (y >= 0 + cameraPos.y || y + static_cast<float>(TerrainGenerator::TILE_SIZE) >= 0 + cameraPos.y) &&
               (y <= Terra::Application::GetApplication()->GetWindowBuffer().y * m_TerrainGeneratorRef->GetCameraRef()->GetZoom() + cameraPos.y));
}

bool Chunk::IsTileValidNeighbor(int x, int y, const TileBitmaskDirection& direction) const
{
    const int neighbourX = x + direction.offsetX;
    const int neighbourY = y + direction.offsetY;

    //vertically out of bounds
    if (neighbourY < 0 || neighbourY >= TerrainGenerator::CHUNK_HEIGHT) return false;
    //Neighbor is inside the same chunk
    if (neighbourX >= 0 && neighbourX < TerrainGenerator::CHUNK_WIDTH) return m_ChunkData[neighbourX][neighbourY].tiletype != TileType::Air;

    //Neighbor is to the left of the current chunk
    if (neighbourX < 0)
    {
        //We make an exception so that it does not cut off weird at the start.
        if (m_ChunkID - 1 < 0) return true;
        return TerrainGenerator::IsTileValid(m_ChunkID - 1, TerrainGenerator::CHUNK_WIDTH - 1,neighbourY);
    }

    //Neighbor is to the right of the current chunk
    if (neighbourX >= TerrainGenerator::CHUNK_WIDTH)
    {
        if (m_ChunkID + 1 >= m_TerrainGeneratorRef->GetChunks().size()) return true;
        return TerrainGenerator::IsTileValid(m_ChunkID + 1, 0,neighbourY);
    }

    return false;
}

glm::vec2 Chunk::getPosition() const
{
    return m_Position;
}
