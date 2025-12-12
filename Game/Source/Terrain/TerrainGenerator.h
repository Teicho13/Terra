#pragma once
#include <array>
#include <PerlinNoise.hpp>

#include "Core/Rendering/Texture.h"

class TerrainGenerator
{
public:
    TerrainGenerator();
    void Render();
private:
    void GeneratePerlinValues();
    
    static constexpr int m_WorldSize = 400;
    double m_NoiseFrequency = 0.04;
    double m_CaveFrequency = 0.08;
    unsigned int m_Seed;

    float m_NoiseHeightMultiplier = 40.f;
    int m_NoiseHeightAddition = 150;
    int m_DirtHeight = 8;

    bool m_GenerateCaves = false;

    std::array<std::array<double,m_WorldSize>,m_WorldSize> m_PerlinValues;
    
    siv::PerlinNoise perlin;
};
