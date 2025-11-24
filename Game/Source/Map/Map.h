#pragma once
#include <array>
#include <string>
#include <fwd.hpp>
#include "Core/Rendering/Texture.h"

class Map
{
public:

    void SetMapData(const std::string& mapPath);
    void SetMapTexture(const std::string& mapTexturePath);

    void DrawMap() const;

    std::array<glm::vec2,4> GetTextureCoordinates(int tileID) const;

    static constexpr int m_MapRows = 23;
    static constexpr int m_MapColumns = 40;
    static constexpr int m_MapTileSize = 32;
    
private:
    std::shared_ptr<Terra::Texture> m_Texture = nullptr;
    std::array<std::array<int,m_MapColumns>,m_MapRows> m_MapData = {};
};
