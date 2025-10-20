#include "Application.h"
#include "Log.h"

// Temporal include
#include <thread>
#include <chrono>
//------------------

namespace RayEngine
{
	void Application::Run()
	{
		Initialize();
		m_IsRunning = true;
		while (m_IsRunning)
		{
			RAY_CORE_INFO("Application Is Running");
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
		Shutdown();
	}
	void Application::Initialize()
	{
		RayEngine::Log::Init();
		RAY_CORE_INFO("Application initializing");
	}
	void Application::Shutdown()
	{
		RAY_CORE_INFO("Shutting down...");
		RayEngine::Log::ShutDown();
	}
}