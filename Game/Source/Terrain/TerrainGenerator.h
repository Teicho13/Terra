#pragma once
#include <array>
#include <PerlinNoise.hpp>
#include "Core/Rendering/Texture.h"

class Chunk;
class TerrainGenerator
{
public:
    TerrainGenerator();
    ~TerrainGenerator();

    void CreateChunks();
    void Render() const;


    //Chunk properties
    
    static constexpr int CHUNK_WIDTH = 12;
    static constexpr int CHUNK_HEIGHT = 128;
    
private:

    //Generation properties
    
    static constexpr int m_WorldSize = 360;
    unsigned int m_Seed;
    bool m_GenerateCaves = true;

    int m_DirtHeight = 8;
    int m_CaveMinDepth = 12;
    
    siv::PerlinNoise perlin;
    float m_NoiseHeightMultiplier = 40.f;
    int m_NoiseHeightAddition = 40;
    double m_NoiseFrequency = 0.04;
    double m_CaveFrequency = 0.08;
    
    
    std::vector<std::unique_ptr<Chunk>> m_Chunks;
};
