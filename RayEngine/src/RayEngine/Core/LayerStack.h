#pragma once
#include "Layer.h"

#include <vector>
#include <memory>
#include <exception>


namespace RayEngine
{
	// LayerStack owns Layer instances. Use std::unique_ptr<Layer> to transfer ownership.
	// Layers are kept in a single vector; m_LayerInsert separates "layers" and "overlays":
	//   [  layers...  |  overlays...  ]
	class LayerStack
	{
	public:
		LayerStack() = default;
		~LayerStack();

		// Transfer ownership of a layer into the stack.
		// - layers are inserted before the insertion index
		void PushLayer(std::unique_ptr<Layer> layer);
		// Transfer ownership of an overlay (appends to the end)
		void PushOverlay(std::unique_ptr<Layer> overlay);

		// Remove by pointer 
		bool RemoveLayer(Layer* layer) noexcept;
		bool RemoveLayer(std::string_view name) noexcept;

		// Pop by pointer (returns ownership)
		[[nodiscard]] std::unique_ptr<Layer> PopLayer(Layer* layer) noexcept;
		[[nodiscard]] std::unique_ptr<Layer> PopLayer(std::string_view name) noexcept;

		// Helpers
		[[nodiscard]] bool Contains(const Layer * layer) const noexcept;
		[[nodiscard]] std::size_t Size() const noexcept;
		void Clear() noexcept; // Detach and destroy all layers

		using iterator = std::vector<std::unique_ptr<Layer>>::iterator;
		using const_iterator = std::vector<std::unique_ptr<Layer>>::const_iterator;

		[[nodiscard]] iterator begin() { return m_Layers.begin(); }
		[[nodiscard]] iterator end() { return m_Layers.end(); }
		[[nodiscard]] const_iterator begin() const { return m_Layers.begin(); }
		[[nodiscard]] const_iterator end() const { return m_Layers.end(); }

	private:
		// Helper to find index by raw pointer; returns npos if not found
		[[nodiscard]] std::size_t FindIndexByPointer(const Layer* layer) const noexcept;
		[[nodiscard]] std::size_t FindIndexByName(std::string_view name) const noexcept;

		void RollbackLayer(const Layer* rawLayer) noexcept;

	private:
		std::vector<std::unique_ptr<Layer>> m_Layers;
		std::size_t m_LayerInsert = 0;
	};
}