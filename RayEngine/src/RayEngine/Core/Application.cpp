#include "Application.h"
#include "Log.h"
#include "Profiler.h"

#include <thread>
#include <chrono>
#include <iostream>
#include <utility>
#include <cassert>
#include <memory>

namespace RayEngine
{
	// initialize owned resources so callers can PushLayer before Run()
	Application::Application()
		: m_Time()
		, m_IsRunning(false)
		, m_LayerStack(std::make_unique<LayerStack>())
	{
	}

	/*
		Main loop contract / documentation (summary):
		- ApplyPending() is called at the top of each frame. All layer mutations requested
		  via the Async APIs are executed here on the main thread.
		- After ApplyPending(), the application updates the Time (Tick) and calls OnUpdate
		  on every layer in the live LayerStack (no snapshot required).
		- Any code that needs to mutate the LayerStack during a frame (including inside
		  OnUpdate/OnAttach handlers) must use the Async APIs. Direct mutation of LayerStack
		  during the update loop is undefined for iteration safety.
		- PopLayerAsync provides a callback that receives the popped ownership on the main
		  thread so callers can reuse the layer object if needed.
	*/

	[[nodiscard]] bool Application::Run()
	{
		RAY_PROFILE_FUNCTION();

		m_Time.Reset();
		m_IsRunning.store(true);

		RAY_CORE_INFO("Application started");

		// Main loop
		while (m_IsRunning.load())
		{
			RAY_PROFILE_SCOPE("MainLoopTick");

			// Apply all pending layer operations that were requested from other threads
			// or during previous frames. This must run before we iterate/update layers.
			ApplyPending();

			// Delta time in seconds (float)
			m_Time.Tick();
			const float deltaTime = m_Time.GetDeltaSecondsf();

			// Iterate live LayerStack directly. Mutations during the frame must be enqueued.
			if (m_LayerStack)
			{
				for (auto& uptr : *m_LayerStack) // iterates std::unique_ptr<Layer>&
				{
					if (!uptr) continue;
					try
					{
						uptr->OnUpdate(deltaTime);
					}
					catch (const std::exception& e)
					{
						RAY_CORE_ERROR(std::string("[Application] Layer OnUpdate() threw: ") + e.what());
					}
					catch (...)
					{
						RAY_CORE_ERROR("[Application] Layer OnUpdate() threw unknown exception");
					}
				}
			}

			// small sleep to avoid busy-loop while you refine frame timing / message pump
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
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

	// --- synchronous operations (main-thread expected) ---
	void Application::PushLayer(std::unique_ptr<Layer> layer)
	{
		if (m_LayerStack && layer)
			m_LayerStack->PushLayer(std::move(layer));
	}

	void Application::PushOverlay(std::unique_ptr<Layer> overlay)
	{
		if (m_LayerStack && overlay)
			m_LayerStack->PushOverlay(std::move(overlay));
	}

	LayerStack& Application::GetLayerStack() noexcept
	{
		assert(m_LayerStack && "LayerStack must be initialized");
		return *m_LayerStack;
	}

	// --- async (thread-safe) request API ---
	void Application::PushLayerAsync(std::unique_ptr<Layer> layer) noexcept
	{
		if (!layer) return;

		// Wrap the unique_ptr into a shared_ptr so the lambda we store in m_PendingOps
		// remains CopyConstructible (std::function requires copyable targets).
		auto holder = std::make_shared<std::unique_ptr<Layer>>(std::move(layer));

		std::lock_guard lock(m_PendingMutex);
		m_PendingOps.emplace_back([this, holder]() mutable {
			if (m_LayerStack && holder && *holder)
			{
				m_LayerStack->PushLayer(std::move(*holder));
			}
		});
	}

	void Application::PushOverlayAsync(std::unique_ptr<Layer> overlay) noexcept
	{
		if (!overlay) return;

		auto holder = std::make_shared<std::unique_ptr<Layer>>(std::move(overlay));

		std::lock_guard lock(m_PendingMutex);
		m_PendingOps.emplace_back([this, holder]() mutable {
			if (m_LayerStack && holder && *holder)
			{
				m_LayerStack->PushOverlay(std::move(*holder));
			}
		});
	}

	void Application::RemoveLayerAsync(Layer* layer) noexcept
	{
		if (!layer) return;
		std::lock_guard lock(m_PendingMutex);
		m_PendingOps.emplace_back([this, layer]() {
			if (m_LayerStack) m_LayerStack->RemoveLayer(layer);
		});
	}

	void Application::PopLayerAsync(Layer* layer, PopCallback cb) noexcept
	{
		if (!layer)
		{
			if (cb) cb(nullptr);
			return;
		}
		std::lock_guard lock(m_PendingMutex);
		// Capture `cb` by value (copy) instead of using a move-capture; this keeps the lambda copyable.
		m_PendingOps.emplace_back([this, layer, cb]() mutable {
			if (!m_LayerStack)
			{
				if (cb) cb(nullptr);
				return;
			}
			auto popped = m_LayerStack->PopLayer(layer); // returns unique_ptr
			if (cb) cb(std::move(popped));
		});
	}

	// Apply pending ops on main thread. Safe point to mutate LayerStack.
	void Application::ApplyPending() noexcept
	{
		std::vector<std::function<void()>> ops;
		{
			std::lock_guard lock(m_PendingMutex);
			ops.swap(m_PendingOps);
		}
		for (auto& op : ops)
		{
			try
			{
				op();
			}
			catch (const std::exception& e)
			{
				RAY_CORE_ERROR(std::string("[Application] pending op threw: ") + e.what());
			}
			catch (...)
			{
				RAY_CORE_ERROR("[Application] pending op threw unknown exception");
			}
		}
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