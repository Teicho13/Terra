#include "Player.h"

#include <algorithm>
#include <iostream>

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

void Player::Jump()
{
    if (m_IsGrounded)
    {
        m_Velocity.y = m_JumpForce;
        m_IsGrounded = false; 
    }
   
}

void Player::Update(const float dt)
{
    ProccessInput(dt);
    Move(dt);
    CollisionCheck();
}

void Player::Draw()
{
    m_Sprite.Draw();
}

Terra::Sprite& Player::GetSprite()
{
    return m_Sprite;
}

void Player::CollisionCheck()
{
    if (!m_TerrainGenRef) return;
    
    const auto position = m_Sprite.GetPosition();
    const int CurrentChunk = static_cast<int>(position.x / (TerrainGenerator::CHUNK_WIDTH * TerrainGenerator::TILE_SIZE));

    const int TilePositionX = static_cast<int>(position.x) % (TerrainGenerator::CHUNK_WIDTH * TerrainGenerator::TILE_SIZE);
    const int TilePositionY = static_cast<int>(position.y) % (TerrainGenerator::CHUNK_HEIGHT * TerrainGenerator::TILE_SIZE);
    

    int TileIndexX = TilePositionX / TerrainGenerator::TILE_SIZE;
    int TileIndexY = TilePositionY / TerrainGenerator::TILE_SIZE;

    //Exit out if invalid Index.
    if (TileIndexX < 0 || TileIndexX > TerrainGenerator::CHUNK_WIDTH - 1 || TileIndexY < 0 || TileIndexY > TerrainGenerator::CHUNK_HEIGHT - 1)
    {
        return;
    }

    const int TileValue = m_TerrainGenRef->GetChunks()[CurrentChunk]->m_ChunkData[TileIndexX][TileIndexY];

    if (TileValue == static_cast<int>(TileType::Grass))
    {
        m_IsGrounded = true;
        auto oldPosition = m_Sprite.GetPosition();
        oldPosition.y = static_cast<float>(TilePositionY);
        m_Sprite.SetPosition(oldPosition);
    }

    if (TileValue == static_cast<int>(TileType::Air))
    {
        m_IsGrounded = false;
    }
}

void Player::Move(float dt)
{
    if (m_Velocity.x != 0.f)
    {
        auto oldPosition = m_Sprite.GetPosition();
        oldPosition.x += m_Velocity.x * dt;
        m_Sprite.SetPosition(oldPosition);
    }

    if (!m_IsGrounded)
    {
        m_Velocity.y += m_Gravity * dt;
        auto oldPosition = m_Sprite.GetPosition();
        oldPosition.y += m_Velocity.y * dt;
        m_Sprite.SetPosition(oldPosition);
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
