#pragma once
namespace Terra
{
    class DeltaTime
    {
    public:
        DeltaTime(double TargetFPS);

        void UpdateTime();
        void ReduceAccumulation();
        
        double Get() const;
        bool ShouldUpdate() const;
        
    private:
        double m_FrameRate;
        const double m_FrameTime;
        
        double m_Accumulator;
        double m_LastTime;
    };
}

