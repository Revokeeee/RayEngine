#include "Application.h"
#include "Log.h"
#include "Profiler.h"

#include <thread>
#include <chrono>
#include <iostream>

namespace RayEngine
{
	[[nodiscard]] bool Application::Run()
	{
		RAY_PROFILE_FUNCTION();

		if (!Initialize())
		{
			// Initialization failed
			std::cerr << "Application initialization failed\n";
			Shutdown();
			return false;
		}

		m_IsRunning.store(true);
		RAY_CORE_INFO("Application started");

		// Main loop
		while (m_IsRunning.load())
		{
			RAY_PROFILE_SCOPE("MainLoopTick");
			RAY_CORE_INFO("Application is running");

			// Simulate work by sleeping for a short duration
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}

		RAY_CORE_INFO("Application stopping");
		Shutdown();
		return true;
	}

	[[nodiscard]] bool Application::Initialize() noexcept
	{
		// Initialize logging first.
		try
		{
			Log::Init();
			RAY_CORE_INFO("Application initializing");
			return true;
		}
		catch (...)
		{
			// Try a minimal fallback to stderr if logging isn't available.
			try
			{
				RAY_CORE_ERROR("Log initialization failed");
			}
			catch (...)
			{
				std::cerr << "Log::Init() failed and logging is not available\n";
			}
			return false;
		}
	}

	void Application::Shutdown() noexcept
	{
		RAY_PROFILE_FUNCTION();
		RAY_CORE_INFO("Shutting down...");
		Log::ShutDown();
	}

	[[nodiscard]] bool Application::IsRunning() const noexcept
	{
		return m_IsRunning.load();
	}

	void Application::Stop() noexcept
	{
		m_IsRunning.store(false);
	}
}