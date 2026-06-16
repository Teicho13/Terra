#include "Player.h"

#include "Core/FileIO.h"
#include "Core/ResourceManager.h"

Player::Player(const std::string& sprite) : m_AnimatedSprite(sprite,1)
{
    m_AnimatedSprite.SetScale({32.f,32.f,0.f});
    m_AnimatedSprite.SetFlipX(true);
    m_AnimatedSprite.SetPosition({32.f,32.f,0.f});
    auto& anim = m_AnimatedSprite.GetAnimation();
    anim.SetLooped(true);
    anim.SetFrameSpeed(20.f);
    CreatePlayerAnimations();
    SwapAnimation("Idle");
    anim.Play();
}

void Player::Update(const float dt)
{
    m_AnimatedSprite.Update(dt);
}

void Player::Draw() const
{
    m_AnimatedSprite.Draw();
}

void Player::CreatePlayerAnimations()
{
    m_Animations[0].Name = "Idle";
    m_Animations[0].Framecount = 4;
    m_Animations[0].Texture = Terra::ResourceManager::GetInstance().GetTexture(Terra::FileIO::GetScuffedMonFile( R"(Characters\Player\player-idle.png)"));
}

bool Player::SwapAnimation(const char* animationName)
{
    //Find struct with the given name in our animations array
    
    const auto it = std::ranges::find_if(m_Animations,[animationName] (const AnimationClip& a)
    {
       return a.Name == animationName;
    });

    if (it != m_Animations.end())
    {
        // If found replace texture and update frame count.

        m_AnimatedSprite.GetAnimation().Reset();
        m_AnimatedSprite.ReplaceTexture(it->Texture);
        m_AnimatedSprite.GetAnimation().SetFrameCount(it->Framecount);
        m_AnimatedSprite.m_CurrentAnimation = &(*it);

        return true;
    }

    return false;
}

Terra::Sprite& Player::GetSprite()
{
    return m_AnimatedSprite;
}