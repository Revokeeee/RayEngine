#pragma once

#include <atomic>
#include <memory>
#include <mutex>
#include <vector>
#include <functional>

#include "LayerStack.h"
#include "Time.h"

namespace RayEngine
{

	// Application owns the LayerStack and a Time instance.
	// Use the async APIs (PushLayerAsync / PushOverlayAsync / RemoveLayerAsync / PopLayerAsync)
	// to safely request layer mutations from any thread or from inside layer callbacks.
	// Pending requests are applied on the main thread at the start of each frame via ApplyPending().
	class Application final
	{
	private:
		Application();

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

		// Initialization / main loop
		[[nodiscard]] bool Initialize() noexcept;
		[[nodiscard]] bool Run();
		[[nodiscard]] bool IsRunning() const noexcept;
		void Stop() noexcept;

		// Synchronous layer helpers (direct, main-thread only).
		// These forward directly to the LayerStack and call OnAttach/OnDetach immediately.
		void PushLayer(std::unique_ptr<Layer> layer);
		void PushOverlay(std::unique_ptr<Layer> overlay);
		LayerStack& GetLayerStack() noexcept;

		// Thread-safe (async) layer request API.
		// Call these from any thread or from inside layer code to schedule changes.
		// Requests are executed on main thread at the next ApplyPending() call (top of frame).
		void PushLayerAsync(std::unique_ptr<Layer> layer) noexcept;
		void PushOverlayAsync(std::unique_ptr<Layer> overlay) noexcept;
		void RemoveLayerAsync(Layer* layer) noexcept; // schedule destruction/removal
		// PopLayerAsync returns ownership to caller via callback executed on the main thread.
		using PopCallback = std::function<void(std::unique_ptr<Layer>)>;
		void PopLayerAsync(Layer* layer, PopCallback cb = nullptr) noexcept;

	private:
		void Shutdown() noexcept;

		// ApplyPending executes all queued requests on the main thread.
		// It must be called from the main thread (Run() calls it at the top of each frame).
		void ApplyPending() noexcept;

	private:
		Time m_Time;
		std::atomic_bool m_IsRunning = false;

		// Owned layer stack
		std::unique_ptr<LayerStack> m_LayerStack;

		// Pending operations (thread-safe queue of lambdas). Lambdas execute on main thread.
		mutable std::mutex m_PendingMutex;
		std::vector<std::function<void()>> m_PendingOps;
	};
}