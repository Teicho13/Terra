#include "ChunkManager.h"
#include "Core/Rendering/Camera.h"
#include "Core/ResourceManager.h"
#include "PerlinNoise.hpp"

ChunkManager::ChunkManager()
{

}

ChunkManager::~ChunkManager()
{
    m_CameraRef = nullptr;
    m_Texture.reset();
}

void ChunkManager::CreateChunks()
{
   for (int x = 0; x < MAX_CHUNKS_WIDTH; ++x)
   {
       const glm::vec2 chunkPos = glm::vec2(x * Chunk::MAX_TILES_WIDTH * 16, 0 * Chunk::MAX_TILES_HEIGHT * 16);
       m_Chunks.emplace_back(std::make_unique<Chunk>(chunkPos,this,x));
   }
}

void ChunkManager::RenderChunks()
{
    for (auto& chunk : m_Chunks)
    {
        chunk->Render(m_Texture);
    }
}

Terra::Camera* ChunkManager::GetCameraRef() const
{
    return m_CameraRef;
}

Terra::Texture* ChunkManager::GetTextureRef() const
{
    return m_Texture.get();
}

void ChunkManager::SetTerrainTexture(const std::string& TerrainTexturePath)
{
    m_Texture.reset();
    
    m_Texture = Terra::ResourceManager::GetInstance().GetTexture(TerrainTexturePath);
}

void ChunkManager::SetActiveCamera(Terra::Camera* camera)
{
    m_CameraRef = camera;
}
