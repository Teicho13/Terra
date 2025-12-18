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
    Leaf
};

namespace Terra {class Camera;}
class Chunk;
class TerrainGenerator
{
public:
    TerrainGenerator();
    ~TerrainGenerator();

    void CreateChunks();
    void Render() const;
    bool IsChunkInView(float x) const;

    void GenerateTree(int chunkIndex,int x, int y);
    
    Terra::Texture* GetTextureRef() const;
    Terra::Camera* GetCameraRef() const;

    void SetTerrainTexture(const std::string& TerrainTexturePath);
    void SetActiveCamera(Terra::Camera* camera);

    //Chunk properties
    
    static constexpr int CHUNK_WIDTH = 12;
    static constexpr int CHUNK_HEIGHT = 228;
    static constexpr int TILE_SIZE = 16;

private:

    //Generation properties
    
    static constexpr int m_WorldSize = 360;
    unsigned int m_Seed;
    bool m_GenerateCaves = true;

    int m_DirtHeight = 8;
    int m_CaveMinDepth = 12;

    //Value represents top value eg 1 out of value.
    int TreeSpawnChance = 50;

    //Perlin Noise properties
    
    siv::PerlinNoise perlin;
    float m_NoiseHeightMultiplier = 30.f;
    int m_NoiseHeightAddition = 60;
    double m_NoiseFrequency = 0.02;
    double m_CaveFrequency = 0.08;

    Terra::Camera* m_CameraRef = nullptr;
    std::shared_ptr<Terra::Texture> m_Texture = nullptr;
    
    std::vector<std::unique_ptr<Chunk>> m_Chunks;
    
    std::mt19937 gen;
};
