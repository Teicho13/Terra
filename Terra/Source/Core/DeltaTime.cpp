#include "DeltaTime.h"
#include <GLFW/glfw3.h>

namespace Terra
{
    DeltaTime::DeltaTime(double TargetFPS)
        : m_FrameRate(TargetFPS), m_FrameTime(1.0 / TargetFPS), m_Accumulator(0.0), m_LastTime(0.0)
    {
    }

    void DeltaTime::UpdateTime()
    {
        double CurrentTime = glfwGetTime();
        double DeltaTime = CurrentTime - m_LastTime;
        m_LastTime = CurrentTime;

        m_Accumulator += DeltaTime;
    }

    bool DeltaTime::ShouldUpdate() const
    {
        return m_Accumulator >= m_FrameTime;
    }

    void DeltaTime::ReduceAccumulation()
    {
        m_Accumulator -= m_FrameTime;
    }

    float DeltaTime::Get() const
    {
        return static_cast<float>(m_FrameTime);
    }
}
