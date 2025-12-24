#pragma once
#include "Core/Sprite.h"

class Player
{
public:
    Player(const std::string& SpriteTexturePath);
    ~Player() = default;
    
    void Draw();
    Terra::Sprite& GetSprite();
    
private:
    Terra::Sprite m_Sprite;
};
