#include "TerrainGenerator.h"
#include <random>

#include "PerlinNoise.hpp"
#include "Core/ResourceManager.h"
#include "Core/Rendering/Renderer.h"

#include <vec3.hpp>
#include <vec4.hpp>

TerrainGenerator::TerrainGenerator()
{
    std::random_device rd;
    std::mt19937 gen(rd());

    // Generate a random float to use as a seed
    
    std::uniform_real_distribution<float> dist(0.f, 9999.f);
    m_Seed = static_cast<unsigned int>(dist(gen));

    //const siv::PerlinNoise::seed_type seed = m_Seed;
    const siv::PerlinNoise::seed_type seed = 1234u;
    perlin.reseed(seed);

    GeneratePerlinValues();
}

void TerrainGenerator::Render()
{
    glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    for (int i = 0; i < m_WorldSize; ++i)
    {
        float height = perlin.noise2D_01(i * m_NoiseFrequency,  m_NoiseFrequency) * m_NoiseHeightMultiplier + m_NoiseHeightAddition;
        for (int j = 0; j < height; ++j)
        {
            if (j < height - m_DirtHeight)
            {
                color = {0.3f,0.3f,0.3f,1.f};
            }
            else if (j < height - 1)
            {
                color = {0.3f,0.1f,0.f,1.f};
            }
            else
            {
                color = {0.f,1.f,0.f,1.f};
            }

            if (m_GenerateCaves)
            {
                if (m_PerlinValues[i][j] > 0.2)
                {
                    Terra::Renderer::DrawQuad(glm::vec3(i,j ,0.f),glm::vec3(1.f,1.f,0.f),color);   
                }
            }
            else
            {
                Terra::Renderer::DrawQuad(glm::vec3(i,j ,0.f),glm::vec3(1.f,1.f,0.f),color);   
            }
            
        }
    }
}

void TerrainGenerator::GeneratePerlinValues()
{
    for (int x = 0; x < m_WorldSize; ++x)
    {
        for (int y = 0; y < m_WorldSize; ++y)
        {
            double perlinValue = perlin.noise2D_01((x) * m_CaveFrequency, (y) * m_CaveFrequency);
            m_PerlinValues[x][y] = perlinValue;
        }
    }
}
