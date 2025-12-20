#pragma once
#include <PerlinNoise.hpp>
#include "Core/Rendering/Texture.h"

enum class TileType
{
    Air = 0,
    Grass,
    Dirt,
    Stone,
    Water,
    Bark,
    Leaf,
    Ore,
    Ore2,
    Ore3
};

namespace Terra {class Camera;}
class Chunk;
class TerrainGenerator
{
public:
    TerrainGenerator();
    ~TerrainGenerator();

    void StartGeneration();
    void CreateChunks();
    void GenerateOres() const;
    void Render() const;
    bool IsChunkInView(float x) const;
    
    Terra::Texture* GetTextureRef() const;
    Terra::Camera* GetCameraRef() const;
    std::vector<std::unique_ptr<Chunk>>& GetChunks();

    void SetTerrainTexture(const std::string& TerrainTexturePath);
    void SetActiveCamera(Terra::Camera* camera);

    //Chunk properties
    
    static constexpr int CHUNK_WIDTH = 16;
    static constexpr int CHUNK_HEIGHT = 128;
    static constexpr int TILE_SIZE = 16;

private:

    //Generation properties
    
    static constexpr int m_WorldSize = 360;
    unsigned int m_Seed;
    
    int m_DirtHeight = 8;
    int m_CaveMinDepth = 12;

    bool m_GenerateCaves = true;
    double m_CaveFrequency = 0.08;
    double m_CaveSizeLimit = 0.25;

    bool m_GenerateOres = true;
    
    double m_OreFrequency = 0.1;
    double m_OreSizeLimit = 0.75;

    int m_MaxOre2Height = 30;
    double m_Ore2Frequency = 0.1;
    double m_Ore2SizeLimit = 0.8;

    int m_MaxOre3Height = 20;
    double m_Ore3Frequency = 0.1;
    double m_Ore3SizeLimit = 0.85;
    
    siv::PerlinNoise perlin;
    float m_NoiseHeightMultiplier = 30.f;
    int m_NoiseHeightAddition = 60;
    double m_NoiseFrequency = 0.02;
    
    Terra::Camera* m_CameraRef = nullptr;
    std::shared_ptr<Terra::Texture> m_Texture = nullptr;
    
    std::vector<std::unique_ptr<Chunk>> m_Chunks;

    std::mt19937 m_Generator;
};
