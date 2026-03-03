#include "AnimatedSprite.h"
#include "Core/Rendering/Renderer.h"

namespace Terra
{
    AnimatedSprite::AnimatedSprite(const std::string& texturePath, int FrameCount)
        :Sprite(texturePath)
    {
        m_Animation.Initialize(FrameCount);
    }

    void AnimatedSprite::Update(float deltaTime)
    {
        m_Animation.Update(deltaTime);
    }

    void AnimatedSprite::Draw() const
    {
        Renderer::DrawQuad(m_Transform,m_Texture,m_Animation.GetTextureCoordinates().data(),m_FlipXAxis,m_FlipYAxis);
    }

    Animation& AnimatedSprite::GetAnimation() { return m_Animation; }
}
