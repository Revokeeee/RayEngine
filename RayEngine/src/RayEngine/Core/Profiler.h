#pragma once

#include <string>
#include <string_view>
#include "Time.h"
#include "Log.h"

namespace RayEngine
{
    class Profiler
    {
    public:
        explicit Profiler(std::string_view name) noexcept
            : m_Name(name)
        {
            m_Timer.Reset();
        }

        // Non-copyable and non-movable to avoid double-logging or lost logs.
        Profiler(const Profiler&) = delete;
        Profiler& operator=(const Profiler&) = delete;
        Profiler(Profiler&&) = delete;
        Profiler& operator=(Profiler&&) = delete;

            ~Profiler() noexcept
        {
            try
            {
                double time = m_Timer.ElapsedMilliseconds();
                RAY_CORE_INFO("[PROFILER] {0}: {1:.3f} ms", m_Name, time);
            }
            catch (...)
            {
                // Ensure destructor never throws.
                
                RAY_CORE_ERROR("Logging failed for '%s'/n", m_Name.c_str());

                // In debug builds, abort so developers notice the problem immediately.
                // Remove or guard this if you do not want an immediate termination in debug.
#ifdef RAY_DEBUG;
                std::abort();
#endif
            }
        }

        // Optional: expose elapsed without logging (useful for aggregators/tests).
        [[nodiscard]] double ElapsedMilliseconds() const noexcept
        {
            return m_Timer.ElapsedMilliseconds();
        }

    private:
        std::string m_Name;
        Time m_Timer;
    };
}

// Macros for profiling
#ifdef RAY_DEBUG
#define RAY_PROFILE_SCOPE(name) RayEngine::Profiler timer##__LINE__(name)
#define RAY_PROFILE_FUNCTION() RAY_PROFILE_SCOPE(__FUNCTION__)
#else // Release build: no-op
#define RAY_PROFILE_SCOPE(name)
#define RAY_PROFILE_FUNCTION()
#endif
