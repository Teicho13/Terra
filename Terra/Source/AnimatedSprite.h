#pragma once
#include "Core/Animation.h"
#include "Core/Sprite.h"

struct AnimationClip
{
    std::string Name;
    std::shared_ptr<Terra::Texture> Texture = nullptr;
    int Framecount = 0;
};

namespace Terra
{
    class AnimatedSprite : public Sprite
    {
    public:
        AnimatedSprite(const std::string& texturePath, int FrameCount);

        void Update(float deltaTime) override;
        void Draw() const override;
        
        Animation& GetAnimation();
        AnimationClip* m_CurrentAnimation = nullptr;
    private:
        Animation m_Animation;
    }; 
}


