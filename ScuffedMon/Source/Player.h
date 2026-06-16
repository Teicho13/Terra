#pragma once
#include "AnimatedSprite.h"

class Player
{
public:
    
    Player(const std::string& sprite);
    void Update(float dt);
    void Draw() const;

    void CreatePlayerAnimations();
    bool SwapAnimation(const char* animationName);

    Terra::Sprite& GetSprite();
private:
    Terra::AnimatedSprite m_AnimatedSprite;
    std::array<AnimationClip, 3> m_Animations;
};
