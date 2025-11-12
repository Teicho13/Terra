#include "Animation.h"

namespace Terra
{
    //1 will be subtracted from the maxFrames to account for frame 0
    void Animation::Initialize(int maxFrames)
    {
        maxFrames = maxFrames - 1;
        if (maxFrames < 0)
        {
            m_MaxFrames = 0;
        }
        else
        {
            m_MaxFrames = maxFrames;
        }
    }

    void Animation::Update(float deltaTime)
    {
        if (m_IsPlaying)
        {
            m_Time += m_FrameSpeed * deltaTime;

            if (m_Time >= 1.f)
            {
                m_CurrentFrame++;
                m_Time = 0.f;

                if (m_CurrentFrame > m_MaxFrames)
                {
                    if (m_LoopAnimation)
                    {
                        m_CurrentFrame = 0;
                    }
                    else
                    {
                        m_CurrentFrame = m_MaxFrames;
                        Stop();
                    }
                }
            }
        }
    }

    void Animation::SetLooped(const bool newValue) { m_LoopAnimation = newValue;}
    void Animation::SetFrameSpeed(const float newSpeed) { m_FrameSpeed = newSpeed; }

    void Animation::Play() { m_IsPlaying = true; }
    void Animation::Stop() { m_IsPlaying = false; }
    void Animation::Reset() { m_CurrentFrame = 0; }
    
    bool Animation::IsPlaying() const { return m_IsPlaying; }
    bool Animation::IsLooped() const { return m_LoopAnimation; }
    int Animation::CurrentFrame() const { return m_CurrentFrame;}

    int Animation::MaxFrames() const { return m_MaxFrames + 1; }
}
