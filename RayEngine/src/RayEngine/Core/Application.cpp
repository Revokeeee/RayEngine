#include "Application.h"

// Temporal include
#include <iostream>
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
			std::cout << "RayEngine is running..." << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
		Shutdown();
	}
	void Application::Initialize()
	{
		// Initialization logic goes here
	}
	void Application::Shutdown()
	{
		// Shutdown logic goes here
	}
}