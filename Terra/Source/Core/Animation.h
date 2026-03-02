#pragma once
#include <array>
#include <vec2.hpp>


namespace Terra
{
    class Animation
    {
    public:
        void Initialize(int maxFrames);
        void Update(float deltaTime);
        void SetLooped(bool newValue);
        void SetReverse(bool newValue);
        void SetFrameSpeed(float newSpeed);
        
        void Play();
        void Stop();
        void Reset();

        bool IsPlaying() const;
        bool IsLooped() const;
        bool IsReversed() const;
        int CurrentFrame() const;
        int MaxFrames() const;

        void UpdateTextureCoordinates();
        std::array<glm::vec2,4> GetTextureCoordinates() const;
        
        
    private:
        bool m_IsPlaying = false;
        bool m_LoopAnimation = false;
        bool m_IsReversed = false;
        float m_FrameSpeed = 1.0f;

        int m_MaxFrames = 0;
        int m_CurrentFrame = 0;

        float m_Time = 0.0f;
        
        float m_Offset = 0.0f;
        std::array<glm::vec2,4> m_TextureCoords = {};
    }; 
}

