#include "Animation.h"
#include <glm.hpp>
namespace Terra
{
    //1 will be subtracted from the maxFrames to account for frame 0. eg: 3 = 0,1,2
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

        m_Offset = 1.f / static_cast<float>(m_MaxFrames + 1);
        UpdateTextureCoordinates();
    }

    void Animation::Update(float deltaTime)
    {
        if (m_IsPlaying)
        {
            m_Time += m_FrameSpeed * deltaTime;

            if (m_Time >= 1.f)
            {
                m_CurrentFrame += m_IsReversed ? -1 : 1;
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

                if (m_CurrentFrame < 0)
                {
                    if (m_LoopAnimation)
                    {
                        m_CurrentFrame = m_MaxFrames;
                    }
                    else
                    {
                        m_CurrentFrame = 0;
                        Stop();
                    }
                }

                UpdateTextureCoordinates();
            }
        }
    }

    void Animation::SetLooped(const bool newValue) { m_LoopAnimation = newValue;}

    void Animation::SetReverse(bool newValue) { m_IsReversed = newValue; }

    void Animation::SetFrameSpeed(const float newSpeed) { m_FrameSpeed = newSpeed; }

    void Animation::UpdateTextureCoordinates()
    {
        m_TextureCoords[0] = { (static_cast<float>(m_CurrentFrame) * m_Offset), 0.0f };
        m_TextureCoords[1] = { (static_cast<float>(m_CurrentFrame) * m_Offset), 1.0f };
        m_TextureCoords[2] = { (static_cast<float>(m_CurrentFrame) * m_Offset) + m_Offset, 1.0f };
        m_TextureCoords[3] = { (static_cast<float>(m_CurrentFrame) * m_Offset) + m_Offset, 0.0f };
    }

    std::array<glm::vec2, 4> Animation::GetTextureCoordinates() const
    {
        return m_TextureCoords;
    }

    void Animation::Play() { m_IsPlaying = true; }
    void Animation::Stop() { m_IsPlaying = false; }
    void Animation::Reset() { m_CurrentFrame = 0; }
    
    bool Animation::IsPlaying() const { return m_IsPlaying; }
    bool Animation::IsLooped() const { return m_LoopAnimation; }
    bool Animation::IsReversed() const { return m_IsReversed; }

    int Animation::CurrentFrame() const { return m_CurrentFrame;}

    int Animation::MaxFrames() const { return m_MaxFrames + 1; }
}
