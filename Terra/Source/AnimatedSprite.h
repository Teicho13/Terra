#pragma once
#include "Core/Animation.h"
#include "Core/Sprite.h"

namespace Terra
{
    class AnimatedSprite : public Sprite
    {
    public:
        AnimatedSprite(const std::string& texturePath, int FrameCount);

        void Update(float deltaTime) override;
        void Draw() const override;
        
        Animation& GetAnimation();
    private:
        Animation m_Animation;
    }; 
}


