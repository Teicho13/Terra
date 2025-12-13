#include "TerrainGenerator.h"
#include <random>

#include "PerlinNoise.hpp"
#include "Core/ResourceManager.h"
#include "Core/Rendering/Renderer.h"

#include <vec3.hpp>
#include <vec4.hpp>
#include "Chunk.h"

TerrainGenerator::TerrainGenerator()
{
    std::random_device rd;
    std::mt19937 gen(rd());

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
    glm::vec4 color = glm::vec4(0.f);

    int PerlinAccumulator = 0;
    
    m_Chunks.reserve(chunkAmount);
    for (int i = 0; i < chunkAmount; ++i)
    {
        m_Chunks.emplace_back(std::make_unique<Chunk>(glm::vec2(i * CHUNK_WIDTH,-CHUNK_HEIGHT)));

        for (int x = 0; x < CHUNK_WIDTH; ++x)
        {
            //Random generate a value for the height of the current x position
            float height = perlin.noise2D_01(PerlinAccumulator * m_NoiseFrequency,  m_NoiseFrequency) * m_NoiseHeightMultiplier + m_NoiseHeightAddition;
            for (int y = 0; y < CHUNK_HEIGHT; ++y)
            {
                if (y < height - m_DirtHeight)
                {
                    color = {0.3f,0.3f,0.3f,1.f};
                }
                else if (y < height - 1)
                {
                    color = {0.3f,0.1f,0.f,1.f};
                }
                else
                {
                    color = {0.f,1.f,0.f,1.f};
                }

                if (y > height)
                {
                    color = {0.4f,0.3f,0.6f,1.f};
                    m_Chunks[i]->m_ChunkData[x][y] = color;
                    continue;
                }
                
                if (m_GenerateCaves)
                {
                    double cavePerlin = perlin.noise2D_01(PerlinAccumulator * m_CaveFrequency, y * m_CaveFrequency);
                    if (cavePerlin > 0.2 || (y > height - m_CaveMinDepth))
                    {
                        m_Chunks[i]->m_ChunkData[x][y] = color;  
                    }
                    else
                    {
                        m_Chunks[i]->m_ChunkData[x][y] = {1.f,1.f,0.f};  
                    }
                }
                else
                {
                    m_Chunks[i]->m_ChunkData[x][y] = color;  
                }
                
                
            }

            PerlinAccumulator++;
        }
    }
}

void TerrainGenerator::Render() const
{
    for (const auto& chunk : m_Chunks)
    {
        for (int x = 0; x < CHUNK_WIDTH; ++x)
        {
            for (int y = 0; y < CHUNK_HEIGHT; ++y)
            {
                glm::vec4 color = {chunk->m_ChunkData[x][y],1.f};
                Terra::Renderer::DrawQuad(glm::vec3(chunk->getPosition().x + x, chunk->getPosition().y + y,0.f),glm::vec3(1.f,1.f,0.f),color); 
            }
        }
    }
}
