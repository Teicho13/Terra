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
    //const siv::PerlinNoise::seed_type seed = 1234u; /*<-- Use for testing same seed*/
    perlin.reseed(seed);

    m_OreGenerationSettings.emplace_back(TileType::Copper,0.15,0.8,m_NoiseHeightAddition);
    m_OreGenerationSettings.emplace_back(TileType::Gold,0.15,0.8,m_NoiseHeightAddition - 20);
    m_OreGenerationSettings.emplace_back(TileType::Diamond,0.15,0.85,10);
}

TerrainGenerator::~TerrainGenerator()
{
    m_Chunks.clear();
}

void TerrainGenerator::StartGeneration()
{
    CreateChunks();
    if (m_GenerateOres)
    {
       GenerateOres(); 
    }
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
                    const double caveNoiseValue = perlin.noise2D_01(PerlinAccumulator * m_CaveFrequency, y * m_CaveFrequency);
                    const bool ShouldBeEmpty = caveNoiseValue > m_CaveSizeLimit && (static_cast<float>(y) < height - static_cast<float>(m_CaveMinDepth));
                    tileType = ShouldBeEmpty ? TileType::Air : tileType;
                }
                
                chunk->m_ChunkData[x][y] = static_cast<int>(tileType);  
            }

            PerlinAccumulator++;
        }
    }
}

void TerrainGenerator::GenerateOres()
{
    //for now we loop over every ore type and then the terrain.
    for (size_t i = 0; i < m_OreGenerationSettings.size(); ++i)
    {
        //We reseed for every ore type we generate.
        const siv::PerlinNoise::seed_type tempSeed = m_Seed + static_cast<unsigned int>(123u * i);
        perlin.reseed(tempSeed);
        
        int PerlinAccumulator = 0;
        
        const auto& oreSetting = m_OreGenerationSettings[i];

        for (const auto& chunk : m_Chunks)
        {
            for (int x = 0; x < CHUNK_WIDTH; ++x)
            {
                for (int y = 0; y < CHUNK_HEIGHT; ++y)
                {
                    //Ignore if we are above this value
                    if (y > m_NoiseHeightAddition)
                    {
                        continue;
                    }

                    const double OrePerlinNoiseValue = perlin.noise2D_01(PerlinAccumulator * oreSetting.Frequency, y * oreSetting.Frequency);
                    if (OrePerlinNoiseValue > oreSetting.SizeLimit && y <= oreSetting.MaxGenerationHeight)
                    {
                        chunk->m_ChunkData[x][y] = static_cast<int>(oreSetting.TileType); 
                    }
                }
                PerlinAccumulator++;  
            }
            
        }
    }

    //Put back the original seed
    const siv::PerlinNoise::seed_type seed = m_Seed;
    perlin.reseed(seed);
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

void TerrainGenerator::GetTileInfo(const glm::vec2& position, int& chunkID, int& column, int& row)
{
    const int CurrentChunk = static_cast<int>(position.x / (CHUNK_WIDTH * TILE_SIZE));

    const int TilePositionX = static_cast<int>(position.x) % (CHUNK_WIDTH * TILE_SIZE);
    const int TilePositionY = static_cast<int>(position.y) % (CHUNK_HEIGHT * TILE_SIZE);
    
    int TileIndexX = TilePositionX / TILE_SIZE;
    int TileIndexY = TilePositionY / TILE_SIZE;

    //Exit out if invalid Index.
    if (TileIndexX < 0 || TileIndexX > CHUNK_WIDTH - 1 || TileIndexY < 0 || TileIndexY > CHUNK_HEIGHT - 1)
    {
        chunkID = column = row = - 1;
        return;
    }

    chunkID = CurrentChunk;
    column = TileIndexX;
    row = TileIndexY;
}

bool TerrainGenerator::IsTileValid(int chunkID, int column, int row)
{
    constexpr int chunkAmount = m_WorldSize / CHUNK_WIDTH;
    if (column < 0 || column > CHUNK_WIDTH - 1 || row < 0 || row > CHUNK_HEIGHT - 1 || chunkID < 0 || chunkID > chunkAmount - 1)
    {
        return false;
    }

    return true;
}

glm::vec3 TerrainGenerator::GetPlayerStartingPosition() const
{
    constexpr float startX = 2.f * TILE_SIZE;
    float startY = 0.f;
    for (int y = 0; y < CHUNK_HEIGHT; ++y)
    {
        if (m_Chunks[0]->m_ChunkData[2][y] == static_cast<int>(TileType::Grass))
        {
            startY = static_cast<float>((y + 1) * TILE_SIZE);
        }
    }
    
    return {startX, startY, 0.f};
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
