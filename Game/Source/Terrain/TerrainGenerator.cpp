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
    m_Generator = std::mt19937(rd());

    // Generate a random float to use as a seed
    
    std::uniform_real_distribution dist(0.f, 9999.f);
    m_Seed = static_cast<unsigned int>(dist(m_Generator));

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
    //Amount of chunks to make.
    constexpr int chunkAmount = m_WorldSize / CHUNK_WIDTH;
    
    //Reserve the space so that we dont need to resize while adding all the data.
    m_Chunks.reserve(chunkAmount);

    //We store a counter so that we dont get repeating patterns when going from one chunk to another.
    int PerlinAccumulator = 0;
    
    std::uniform_real_distribution dirtOffsetDist(-1.f, 3.f);
    
    for (int i = 0; i < chunkAmount; ++i)
    {
        m_Chunks.emplace_back(std::make_unique<Chunk>(glm::vec2(i * (CHUNK_WIDTH * TILE_SIZE),0),this));
        const auto& chunk = m_Chunks.back();

        for (int x = 0; x < CHUNK_WIDTH; ++x)
        {
            //Random generate a value for the height of the current x position.
            const float height = static_cast<float>(perlin.noise2D_01(PerlinAccumulator * m_NoiseFrequency,  m_NoiseFrequency))
            * m_NoiseHeightMultiplier + static_cast<float>(m_NoiseHeightAddition);

            //Generate a random offset for the dirt height to add some verity.
            const float dirtOffset = dirtOffsetDist(m_Generator);
            
            for (int y = 0; y < CHUNK_HEIGHT; ++y)
            {

                //If we are above the terrain exit early. 
                if (static_cast<float>(y) > height)
                {
                    chunk->m_ChunkData[x][y] = static_cast<int>(TileType::Air);
                    continue;
                }

                //Check our current tile to determine what tile to place.
                TileType tileType;
                if (static_cast<float>(y) < height - (static_cast<float>(m_DirtHeight) + dirtOffset))
                    tileType = TileType::Stone;
                else if (static_cast<float>(y) < height - 1)
                    tileType = TileType::Dirt;
                else
                    tileType = TileType::Grass;
                
                //Check if we need to generate caves.
                if (m_GenerateCaves)
                {
                    const double cavePerlin = perlin.noise2D_01(PerlinAccumulator * m_CaveFrequency, y * m_CaveFrequency);
                    const bool ShouldBeEmpty = cavePerlin > 0.2 || (static_cast<float>(y) > height - static_cast<float>(m_CaveMinDepth));
                    tileType = ShouldBeEmpty ? tileType : TileType::Air;
                }

                chunk->m_ChunkData[x][y] = static_cast<int>(tileType);  
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

Terra::Texture* TerrainGenerator::GetTextureRef() const
{
    return m_Texture.get();
}

Terra::Camera* TerrainGenerator::GetCameraRef() const
{
    return m_CameraRef;
}

std::vector<std::unique_ptr<Chunk>>& TerrainGenerator::GetChunks()
{
    return m_Chunks;
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
