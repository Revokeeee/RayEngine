#pragma once

#include <chrono>

namespace RayEngine
{
	// Simple Timer class for measuring elapsed time
    class Time
    {
    public:
        using Clock = std::chrono::steady_clock;
        
    public:
        Time() 
        {
            Reset();
        }

        void Reset() noexcept
        {
            m_StartTime = Clock::now();
        }

        [[nodiscard]] double ElapsedMilliseconds() const noexcept
        {
            auto now = Clock::now();
            return std::chrono::duration<double, std::milli>(now - m_StartTime).count();
        }

        [[nodiscard]] double ElapsedSeconds() const noexcept
        {
            auto now = Clock::now();
            return std::chrono::duration<double>(now - m_StartTime).count();
        }

    private:
        std::chrono::time_point<Clock> m_StartTime;
    };
}
