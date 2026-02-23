#include "Player.h"

#include <algorithm>
#include <iostream>

#include "../../CameraManager.h"
#include "../../Terrain/Chunk.h"
#include "../../Terrain/TerrainGenerator.h"
#include "Core/Application.h"
#include "Core/Rendering/Renderer.h"

Player::Player(const std::string& SpriteTexturePath)
    :m_Sprite(SpriteTexturePath), m_Velocity(0.f,0.f)
{
}

void Player::SetTerrainGeneratorRef(TerrainGenerator* terrainGeneratorRef)
{
    m_TerrainGenRef = terrainGeneratorRef;
}

void Player::SetCameraManagerRef(CameraManager* cameraRef)
{
    m_CameraRef = cameraRef;
}

void Player::Jump()
{
    if (m_IsGrounded)
    {
        m_Velocity.y = m_JumpForce;
        m_IsGrounded = false; 
    }
   
}

void Player::Clicked(const float clickedX, const float clickedY, bool hasRightClicked) const
{
    //First we get the tile we clicked on.
    if (!m_TerrainGenRef || !m_CameraRef) return;
    
    //Convert screenspace position to world space
    const glm::vec2 worldposition = m_CameraRef->ScreenToWorldPosition(clickedX, clickedY);
    
    int chunk,x,y;
    TerrainGenerator::GetTileInfo(worldposition,chunk,x,y);
    if (chunk == -1 || x == -1 || y == -1) return;
    
    const auto newTileType = hasRightClicked ? TileType::Air : TileType::Dirt;
    m_TerrainGenRef->GetChunks()[chunk]->m_ChunkData[x][y].tiletype = newTileType;
}

glm::vec2 Player::GetVelocity() const
{
    return m_Velocity;
}

void Player::Update(const float dt)
{
    ProccessInput(dt);
    Move(dt);
}

void Player::Draw()
{
    m_Sprite.Draw();
}

Terra::Sprite& Player::GetSprite()
{
    return m_Sprite;
}

bool Player::CollisionCheck(const glm::vec2& position) const
{
    if (!m_TerrainGenRef) return false;
    
    const glm::vec2 size = m_Sprite.GetScale();

    //All corners of the sprite
    std::array<glm::vec2, 4> positions;
    positions[0] = glm::vec2(position.x,position.y);
    positions[1] = glm::vec2(position.x+size.x,position.y);
    positions[2] = glm::vec2(position.x+size.x,position.y+size.y);
    positions[3] = glm::vec2(position.x,position.y+size.y);

    //If any of the positions are invalid or are not air we collided with something.
    for (int i = 0; i < 4; ++i)
    {
        int chunk,x,y;
        TerrainGenerator::GetTileInfo(positions[i],chunk,x,y);
        
        if (m_TerrainGenRef->IsTileValid(chunk,x,y))
        {
            if (m_TerrainGenRef->GetChunks()[chunk]->m_ChunkData[x][y].tiletype != TileType::Air)
            {
                return true;
            }
        }
        else
        {
            return true;
        }
    }
    
    return false;
}

void Player::Move(float dt)
{
    if (m_Velocity.x != 0.f)
    {
        auto oldPosition = m_Sprite.GetPosition();
        oldPosition.x += m_Velocity.x * dt;
        if (!CollisionCheck(oldPosition))
        {
            m_Sprite.SetPosition(oldPosition); 
        }
        else
        {
            m_Velocity.x = 0.f;
        }
    }

    m_Velocity.y += m_Gravity * dt;
    auto oldPosition = m_Sprite.GetPosition();
    oldPosition.y += m_Velocity.y * dt;
    if (!CollisionCheck(oldPosition))
    {
        m_Sprite.SetPosition(oldPosition); 
    }
    else
    {
        m_Velocity.y = 0.f;
        m_IsGrounded = true;
    }

    //Check so that we wont go out of bounds
    if (m_Sprite.GetPosition().x <= 0.f)
    {
        m_Sprite.SetPosition({0,m_Sprite.GetPosition().y,m_Sprite.GetPosition().z});
        m_Velocity.x = 0.f;
    }
}

void Player::ProccessInput(float dt)
{
    const auto window = Terra::Application::GetApplication()->GetWindow()->GetWindow();

    //Acceleration
    
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        m_Velocity.x = m_Velocity.x + (-m_MaxSpeed * m_Acceleration *  dt);
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        m_Velocity.x = m_Velocity.x + (m_MaxSpeed * m_Acceleration *  dt);
    }
    else
    {
        //Deceleration
        
        if (m_Velocity.x < 0.f)
        {
            m_Velocity.x = m_Velocity.x + (m_MaxSpeed * m_Acceleration *  dt);
            m_Velocity.x = std::min(m_Velocity.x, 0.f);
        }
        else if (m_Velocity.x > 0.f)
        {
            m_Velocity.x = m_Velocity.x - (m_MaxSpeed * m_Acceleration *  dt);
            m_Velocity.x = std::max(m_Velocity.x, 0.f);
        }
    }
    
    m_Velocity.x = std::clamp(m_Velocity.x, -m_MaxSpeed, m_MaxSpeed);
}
