#pragma once

#include <chrono>

namespace RayEngine
{
	// Simple Timer class for frame timing and elapsed time measurement.
	class Time
	{
	public:
		using Clock = std::chrono::steady_clock;
		using TimePoint = std::chrono::time_point<Clock>;

		Time() noexcept
		{
			Reset();
		}

		// Reset start and last time to now.
		void Reset() noexcept
		{
			m_StartTime = Clock::now();
			m_LastTime = m_StartTime;
			m_DeltaSeconds = 0.0;
			m_ElapsedSeconds = 0.0;
		}

		// Advance the internal clock by one frame; call once per frame.
		void Tick() noexcept
		{
			const TimePoint now = Clock::now();
			m_DeltaSeconds = std::chrono::duration<double>(now - m_LastTime).count();
			m_LastTime = now;
			m_ElapsedSeconds = std::chrono::duration<double>(now - m_StartTime).count();
		}

		// Time since last Tick() in seconds (double)
		[[nodiscard]] double GetDeltaSeconds() const noexcept { return m_DeltaSeconds; }
		// Convenience float overload for game code
		[[nodiscard]] float GetDeltaSecondsf() const noexcept { return static_cast<float>(m_DeltaSeconds); }

		// Total time since Reset() in seconds
		[[nodiscard]] double GetElapsedSeconds() const noexcept { return m_ElapsedSeconds; }
		[[nodiscard]] double ElapsedSeconds() const noexcept { return GetElapsedSeconds(); }
		[[nodiscard]] double ElapsedMilliseconds() const noexcept
		{
			return std::chrono::duration<double, std::milli>(Clock::now() - m_StartTime).count();
		}

	private:
		TimePoint m_StartTime;
		TimePoint m_LastTime;
		double m_DeltaSeconds = 0.0;
		double m_ElapsedSeconds = 0.0;
	};
}
