#pragma once
#include <PerlinNoise.hpp>
#include <vec2.hpp>
#include <vec3.hpp>

#include "Core/Rendering/Texture.h"
#include "Tile.h"

struct OreGenerationSettings
{
    OreGenerationSettings(const TileType tiletype, const double freq, const double sizelimit, const int maxheight)
    : TileType(tiletype), Frequency(freq),SizeLimit(sizelimit), MaxGenerationHeight(maxheight){}

    TileType TileType;
    double Frequency;
    double SizeLimit;
    int MaxGenerationHeight;
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
    void GenerateOres();
    void Render() const;
    void Update(float deltaTime) const;
    bool IsChunkInView(float x) const;
    
    Terra::Texture* GetTextureRef() const;
    Terra::Camera* GetCameraRef() const;
    std::vector<std::unique_ptr<Chunk>>& GetChunks();

    static void GetTileInfo(const glm::vec2& position, int& chunkID, int& column, int& row);
    static bool IsTileValid(int chunkID, int column, int row);
    glm::vec3 GetPlayerStartingPosition() const;

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
    double m_CaveFrequency = 0.07;
    double m_CaveSizeLimit = 0.715;

    bool m_GenerateOres = true;
    std::vector<OreGenerationSettings> m_OreGenerationSettings;
    
    siv::PerlinNoise perlin;
    float m_NoiseHeightMultiplier = 30.f;
    int m_NoiseHeightAddition = 60;
    double m_NoiseFrequency = 0.02;
    
    Terra::Camera* m_CameraRef = nullptr;
    std::shared_ptr<Terra::Texture> m_Texture = nullptr;
    
    std::vector<std::unique_ptr<Chunk>> m_Chunks;

    std::mt19937 m_Generator;
};
