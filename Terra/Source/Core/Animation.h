#pragma once

namespace Terra
{
    class Animation
    {
    public:
        void Initialize(int maxFrames);
        void Update(float deltaTime);
        void SetLooped(bool newValue);
        void SetFrameSpeed(float newSpeed);

        void Play();
        void Stop();
        void Reset();

        bool IsPlaying() const;
        bool IsLooped() const;
        int CurrentFrame() const;
        int MaxFrames() const;
        
    private:
        bool m_IsPlaying = false;
        bool m_LoopAnimation = false;
        float m_FrameSpeed = 1.0f;

        int m_MaxFrames = 0;
        int m_CurrentFrame = 0;

        float m_Time = 0.0f;
    }; 
}

