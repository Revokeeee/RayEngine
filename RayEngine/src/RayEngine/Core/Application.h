#pragma once

#include <atomic>

namespace RayEngine
{

	class Application final
	{
	private: 
		Application() = default;
	public:
		static Application& GetInstance() noexcept
		{
			static Application instance;
			return instance;
		}
		// Non-copyable and non-movable
		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(Application&&) = delete;

		[[nodiscard]] bool Run();
		[[nodiscard]] bool IsRunning() const noexcept;
		void Stop() noexcept;
	private:
		[[nodiscard]] bool Initialize() noexcept;
		void Shutdown() noexcept;
	private:
		std::atomic_bool m_IsRunning = false;
	};
}