#include "TerrainGenerator.h"

#include <random>

#include "PerlinNoise.hpp"
#include "Core/ResourceManager.h"
#include "Core/Rendering/Renderer.h"

#include "Chunk.h"
#include "Core/Application.h"
#include "Core/Rendering/Camera.h"

TerrainGenerator::TerrainGenerator()
{
    std::random_device rd;
    gen = std::mt19937(rd());

    // Generate a random float to use as a seed

    std::uniform_real_distribution<float> dist(0.f, 9999.f);
    m_Seed = static_cast<unsigned int>(dist(gen));

    const siv::PerlinNoise::seed_type seed = m_Seed;
    //const siv::PerlinNoise::seed_type seed = 1234u; <-- Use for testing same seed
    perlin.reseed(seed);
}

TerrainGenerator::~TerrainGenerator()
{
    m_Chunks.clear();
}

void TerrainGenerator::CreateChunks()
{
    constexpr int chunkAmount = m_WorldSize / CHUNK_WIDTH;
    TileType tileType = TileType::Air;

    int PerlinAccumulator = 0;
    std::uniform_real_distribution<float> dist(1.f, static_cast<float>(TreeSpawnChance));
    
    m_Chunks.reserve(chunkAmount);
    for (int i = 0; i < chunkAmount; ++i)
    {
        m_Chunks.emplace_back(std::make_unique<Chunk>(glm::vec2(i * (CHUNK_WIDTH * TILE_SIZE),(-CHUNK_HEIGHT * TILE_SIZE) / 4),this));

        for (int x = 0; x < CHUNK_WIDTH; ++x)
        {
            //Random generate a value for the height of the current x position
            float height = perlin.noise2D_01(PerlinAccumulator * m_NoiseFrequency,  m_NoiseFrequency) * m_NoiseHeightMultiplier + m_NoiseHeightAddition;
            for (int y = 0; y < CHUNK_HEIGHT; ++y)
            {
                if (y < height - m_DirtHeight)
                {
                    tileType = TileType::Stone;
                }
                else if (y < height - 1)
                {
                    tileType = TileType::Dirt;
                }
                else
                {
                    tileType = TileType::Grass;
                }

                if (y > height)
                {
                    if (m_Chunks[i]->m_ChunkData[x][y] != static_cast<int>(TileType::Bark) && m_Chunks[i]->m_ChunkData[x][y] != static_cast<int>(TileType::Leaf))
                    {
                        m_Chunks[i]->m_ChunkData[x][y] = static_cast<int>(TileType::Air);
                    }
                    continue;
                }
                
                if (m_GenerateCaves)
                {
                    double cavePerlin = perlin.noise2D_01(PerlinAccumulator * m_CaveFrequency, y * m_CaveFrequency);
                    if (cavePerlin > 0.2 || (y > height - m_CaveMinDepth))
                    {
                        m_Chunks[i]->m_ChunkData[x][y] = static_cast<int>(tileType);  
                    }
                    else
                    {
                        m_Chunks[i]->m_ChunkData[x][y] = static_cast<int>(TileType::Air);  
                    }
                }
                else
                {
                    m_Chunks[i]->m_ChunkData[x][y] = static_cast<int>(tileType);  
                }
            }

            if (static_cast<unsigned int>(dist(gen)) == 1)
            {
                GenerateTree(i,x, static_cast<int>(height) + 1);
            }
            
            PerlinAccumulator++;
        }
    }
}

void TerrainGenerator::Render() const
{
    for (const auto& chunk : m_Chunks)
    {
        if (IsChunkInView(chunk->getPosition().x))
        {
            chunk->Render(m_Texture);
        }
    }
}

bool TerrainGenerator::IsChunkInView(const float x) const
{
    //If there is no active camera we just render regardless.
    if (!m_CameraRef) return true;

    const auto camPos = GetCameraRef()->GetPosition();
    return ((x >= 0 + camPos.x || x + static_cast<float>(CHUNK_WIDTH * TILE_SIZE) >= 0 + camPos.x) &&
            (x <= Terra::Application::GetApplication()->GetWindowBuffer().x * GetCameraRef()->GetZoomLevel() + camPos.x));
}

void TerrainGenerator::GenerateTree(int chunkIndex, int x, int y)
{
    if (x == 0 || x == CHUNK_WIDTH - 1) return;
    
    std::uniform_real_distribution<float> dist(3.f, 5.f);
    unsigned int TreeHeight = dist(gen);

    //Temp ugly hardcode
    for (int i = 0; i < TreeHeight; ++i)
    {
        m_Chunks[chunkIndex]->m_ChunkData[x][y + i] = static_cast<int>(TileType::Bark);  
    }

    m_Chunks[chunkIndex]->m_ChunkData[x][y + TreeHeight] = static_cast<int>(TileType::Leaf);  
    m_Chunks[chunkIndex]->m_ChunkData[x][y + TreeHeight + 1] = static_cast<int>(TileType::Leaf);  
    m_Chunks[chunkIndex]->m_ChunkData[x][y + TreeHeight + 2] = static_cast<int>(TileType::Leaf);

    m_Chunks[chunkIndex]->m_ChunkData[x + 1][y + TreeHeight] = static_cast<int>(TileType::Leaf);  
    m_Chunks[chunkIndex]->m_ChunkData[x + 1][y + TreeHeight + 1] = static_cast<int>(TileType::Leaf);

    m_Chunks[chunkIndex]->m_ChunkData[x - 1][y + TreeHeight] = static_cast<int>(TileType::Leaf);  
    m_Chunks[chunkIndex]->m_ChunkData[x - 1][y + TreeHeight + 1] = static_cast<int>(TileType::Leaf); 
    
}

Terra::Texture* TerrainGenerator::GetTextureRef() const
{
    return m_Texture.get();
}

Terra::Camera* TerrainGenerator::GetCameraRef() const
{
    return m_CameraRef;
}

void TerrainGenerator::SetTerrainTexture(const std::string& TerrainTexturePath)
{
    m_Texture.reset();
    
    m_Texture = Terra::ResourceManager::GetInstance().GetTexture(TerrainTexturePath); 
}

void TerrainGenerator::SetActiveCamera(Terra::Camera* camera)
{
    m_CameraRef = camera;
}
