#pragma once

namespace RayEngine
{

	class Application
	{
	private: 
		Application() = default;
	public:
		static Application& GetInstance()
		{
			static Application instance;
			return instance;
		}
		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;
		void Run();
	private:
		void Initialize();
		void Shutdown();
	private:
		bool m_IsRunning = false;

	};
}