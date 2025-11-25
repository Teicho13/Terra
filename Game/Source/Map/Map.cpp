#include "Map.h"
#include <fstream>
#include <sstream>
#include <vec2.hpp>
#include <vec3.hpp>

#include "Core/Application.h"
#include "Core/ResourceManager.h"
#include "Core/Rendering/Renderer.h"
#include "Core/Rendering/Camera.h"

void Map::SetMapData(const std::string& mapDataPath)
{
    bool hasReachedWhile = false;

    //Create temporary files for handling strings
    std::ifstream file;
    std::string line;
    std::string final;

    //Open Texture Map
    file.open(mapDataPath);
    
    //Create string with all numbers

    while (std::getline(file, line))
    {
        final.append(line);
        final.append(",");
    }

    int type;
    std::string temp;
    
    //Iterators
    
    int itX = 0;
    int itY = 0;
    
    std::istringstream str_buf {final};

    //Loop over all numbers while ignoring commas.
    while (str_buf >> type)
    {
        hasReachedWhile = true;

        m_MapData[itY][itX] = type;

        if (itX < (m_MapColumns - 1))
        {
            itX++;
        }
        else
        {
            itX = 0;
            if (itY < m_MapRows)
            {
                itY++;
            }
        }

        //Skip Comma characters
        if ((str_buf >> std::ws).peek() == ',')
            str_buf.ignore();
    }

    file.close();
}

void Map::SetMapTexture(const std::string& mapTexturePath)
{
    m_Texture.reset();
    
    m_Texture = Terra::ResourceManager::GetInstance().GetTexture(mapTexturePath);
}

void Map::SetCamera(Terra::Camera* camera)
{
    m_CameraRef = camera;
}

void Map::DrawMap() const
{
    for (int y = 0; y < m_MapRows; y++)
    {
        //OpenGL strikes again
        int flippedY = (m_MapRows - 1 - y);
        
        for (int x = 0; x < m_MapColumns; x++)
        {
            const float posX = static_cast<float>(x * m_MapTileSize);
            const float posY = static_cast<float>(flippedY * m_MapTileSize);

            if (IsWithinBounds(posX, posY))
            {
                Terra::Renderer::DrawQuad(
                 glm::vec3(posX,posY,1.f),
                 glm::vec3(static_cast<float>(m_MapTileSize)),
                 m_Texture,
                 GetTextureCoordinates(m_MapData[y][x]).data()
                 ); 
            }
        }
    }
}

//Make sure we check if the tiles are within the camera bounds so that we dont create vertices when its not needed.
bool Map::IsWithinBounds(float x, float y) const
{
    return ((x >= 0 + m_CameraRef->GetPosition().x || x + m_MapTileSize >= 0 + m_CameraRef->GetPosition().x) &&
                (x <= Terra::Application::GetApplication()->GetWindowBuffer().x + m_CameraRef->GetPosition().x) &&
                (y >= 0 + m_CameraRef->GetPosition().y || y + m_MapTileSize >= 0 + m_CameraRef->GetPosition().y) &&
                (y <= Terra::Application::GetApplication()->GetWindowBuffer().y + m_CameraRef->GetPosition().y));
}

std::array<glm::vec2,4> Map::GetTextureCoordinates(const int tileID) const
{
    //we get the total amount of tiles in the textures.
    const int TextureTileCountX = (m_Texture->GetWidth() / m_MapTileSize);
    const int TextureTileCountY = (m_Texture->GetHeight() / m_MapTileSize);

    //We first get the target tile position.
    //We flip the TargetTileY because openGL starts from the bottom.

    const int TargetTileY = (TextureTileCountY - 1) - (tileID / TextureTileCountX);
    const int TargetTileX = tileID % TextureTileCountX;

    //we calculate the gap in between each frame based on the amount of tiles in each row and column.
    
    const float OffsetX = 1.f / static_cast<float>(TextureTileCountX);
    const float OffsetY = 1.f / static_cast<float>(TextureTileCountY);

    std::array<glm::vec2,4> TextureCoord;
    TextureCoord[0] = { (static_cast<float>(TargetTileX) * OffsetX), (static_cast<float>(TargetTileY) * OffsetY) };
    TextureCoord[1] = { (static_cast<float>(TargetTileX) * OffsetX), (static_cast<float>(TargetTileY) * OffsetY) + OffsetY };
    TextureCoord[2] = { (static_cast<float>(TargetTileX) * OffsetX) + OffsetX, (static_cast<float>(TargetTileY) * OffsetY) + OffsetY };
    TextureCoord[3] = { (static_cast<float>(TargetTileX) * OffsetX) + OffsetX, (static_cast<float>(TargetTileY) * OffsetY) };
    
    return TextureCoord;
}
