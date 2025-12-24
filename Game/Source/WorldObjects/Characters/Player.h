#pragma once
#include "Core/Sprite.h"

class TerrainGenerator;
class Player
{
public:
    Player(const std::string& SpriteTexturePath);
    ~Player() = default;

    void SetTerrainGeneratorRef(TerrainGenerator* terrainGeneratorRef);
    
    void Jump();
    
    void Update(float dt);
    void Draw();
    Terra::Sprite& GetSprite();
    
private:
    void CollisionCheck();
    void Move(float dt);
    void ProccessInput(float dt);
    
    float m_MovementSpeed = 300.f;
    float m_Acceleration = 5.f;
    float m_MaxSpeed = 300.f;
    glm::vec2 m_Velocity;
    float m_JumpForce = 300.f;
    float m_Gravity = -1000.f;
    bool m_IsGrounded = true;
    
    Terra::Sprite m_Sprite;
    TerrainGenerator* m_TerrainGenRef = nullptr;
};
