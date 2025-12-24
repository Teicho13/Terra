#include "Player.h"

Player::Player(const std::string& SpriteTexturePath)
    :m_Sprite(SpriteTexturePath)
{
}

void Player::Draw()
{
    m_Sprite.Draw();
}

Terra::Sprite& Player::GetSprite()
{
    return m_Sprite;
}
