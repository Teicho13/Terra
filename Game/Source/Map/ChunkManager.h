#pragma once
#include <array>
#include <memory>
#include <string>

#include "Chunk.h"

namespace Terra {class Texture; class Camera;}
class ChunkManager
{
public:
    ChunkManager();
    ~ChunkManager();

    void CreateChunks();
    void RenderChunks();

    Terra::Camera* GetCameraRef() const;
    Terra::Texture* GetTextureRef() const;

    void SetTerrainTexture(const std::string& TerrainTexturePath);
    void SetActiveCamera(Terra::Camera* camera);
    
    //Max amount of chunks (width)
    static constexpr int MAX_CHUNKS_WIDTH = 12;

private:
    std::shared_ptr<Terra::Texture> m_Texture = nullptr;
    std::vector<std::unique_ptr<Chunk>> m_Chunks;
    Terra::Camera* m_CameraRef = nullptr;
};
