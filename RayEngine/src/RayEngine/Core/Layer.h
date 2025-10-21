#pragma once

#include <string>
#include <string_view>

namespace RayEngine
{
	// Base class for application layers.
	// - Non-copyable and non-movable (prevent slicing / accidental copies).
	// - Construction is move-friendly.
	class Layer
	{
	public:
		Layer(const std::string name = "Layer")
			: m_Name(std::move(name))
		{
		}
		virtual ~Layer() = default;

		// Prevent copying/moving of the polymorphic base to avoid slicing.
		Layer(const Layer&) = delete;
		Layer& operator=(const Layer&) = delete;
		Layer(Layer&&) = delete;
		Layer& operator=(Layer&&) = delete;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(float deltaTime) {}
		//TODO: ImGui layer
		//virtual void OnImGuiRender() {}
		//TODO: Event system  
		//virtual void OnEvent(Event& event) {}
		[[nodiscard]] const std::string& GetName() const noexcept { return m_Name; }
	protected:
		std::string m_Name;
	};

}