#pragma once
#include "AnimatedSprite.h"
#include "Core/Rendering/Camera.h"

class TerrainGenerator;
class CameraManager;
class Player
{
public:
    Player(const std::string& SpriteTexturePath);
    ~Player() = default;

    void Update(float dt);
    void Draw();

    bool SwapAnimation(const char* animationName);

    void SetTerrainGeneratorRef(TerrainGenerator* terrainGeneratorRef);
    void SetCameraManagerRef(CameraManager* cameraRef);
    void Jump();
    void Clicked(float clickedX, float clickedY, bool hasLeftClicked) const;

    glm::vec2 GetVelocity() const;
    Terra::Sprite& GetSprite();

private:
    void Move(float dt);
    void ProccessInput(float dt);
    void AnimationChecks();
    
    bool CollisionCheck(const glm::vec2& position) const;

    void CreatePlayerAnimations();
    
    float m_Acceleration = 5.f;
    float m_MaxSpeed = 300.f;
    float m_JumpForce = 300.f;
    glm::vec2 m_Velocity;
    
    float m_Gravity = -1000.f;
    bool m_IsGrounded = true;
    
    Terra::AnimatedSprite m_AnimatedSprite;
    TerrainGenerator* m_TerrainGenRef = nullptr;
    CameraManager* m_CameraRef = nullptr;

    std::array<AnimationClip, 3> m_Animations;
};
