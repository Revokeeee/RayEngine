#include "LayerStack.h"
#include "Log.h"

#include <limits>
#include <exception>

namespace RayEngine
{
	static constexpr std::size_t npos = std::numeric_limits<std::size_t>::max();

	LayerStack::~LayerStack()
	{
		Clear();
	}

	void LayerStack::PushLayer(std::unique_ptr<Layer> layer)
	{
		if (!layer)
			return;
		Layer* rawLayer = layer.get();
		m_Layers.emplace(m_Layers.begin() + m_LayerInsert, std::move(layer));
		m_LayerInsert++;
		try
		{
			rawLayer->OnAttach();
		}
		catch (const std::exception& e)
		{
			RAY_CORE_ERROR(std::string("[LayerStack] OnAttach() threw exception: ") + e.what());
			RollbackLayer(rawLayer);
			throw;
		}
		catch (...)
		{
			RAY_CORE_ERROR(std::string("[LayerStack] OnAttach() threw unknown exception"));
			RollbackLayer(rawLayer);
			throw;
		}
	}

	void LayerStack::PushOverlay(std::unique_ptr<Layer> overlay)
	{
		if (!overlay)
			return;

		Layer* rawOverlay = overlay.get();
		m_Layers.emplace_back(std::move(overlay));

		try
		{
			rawOverlay->OnAttach();
		}
		catch (const std::exception& e)
		{
			RAY_CORE_ERROR(std::string("[LayerStack] OnAttach() threw exception: ") + e.what());
			RollbackLayer(rawOverlay);
			throw;
		}
		catch (...)
		{
			RAY_CORE_ERROR(std::string("[LayerStack] OnAttach() threw unknown exception"));
			RollbackLayer(rawOverlay);
			throw;
		}
	}

	bool LayerStack::RemoveLayer(Layer* layer) noexcept
	{
		if(layer == nullptr)
			return false;

		auto index = FindIndexByPointer(layer);
		if (index == npos)
			return false;

		try
		{
			m_Layers[index]->OnDetach();
		}
		catch (const std::exception& e)
		{
			RAY_CORE_ERROR(std::string("[LayerStack] OnDetach() threw exception: ") + e.what());
		}
		catch (...)
		{
			RAY_CORE_ERROR(std::string("[LayerStack] OnDetach() threw unknown exception"));
		}

		m_Layers.erase(m_Layers.begin() + index);

		if (index < m_LayerInsert)
		{
			m_LayerInsert--;
		}
		return true;
	}

	bool LayerStack::RemoveLayer(std::string_view name) noexcept
	{
		auto index = FindIndexByName(name);
		if (index == npos)
			return false;

		try
		{
			m_Layers[index]->OnDetach();
		}
		catch (const std::exception& e)
		{
			RAY_CORE_ERROR(std::string("[LayerStack] OnDetach() threw exception: ") + e.what());
		}
		catch (...)
		{
			RAY_CORE_ERROR(std::string("[LayerStack] OnDetach() threw unknown exception"));
		}

		m_Layers.erase(m_Layers.begin() + index);

		if (index < m_LayerInsert)
		{
			m_LayerInsert--;
		}
		return true;
	}

	std::unique_ptr<Layer> LayerStack::PopLayer(Layer* layer) noexcept
	{
		if (layer == nullptr)
			return nullptr;

		auto index = FindIndexByPointer(layer);
		if (index == npos)
			return nullptr;

		try
		{
			m_Layers[index]->OnDetach();
		}
		catch (const std::exception& e)
		{
			RAY_CORE_ERROR(std::string("[LayerStack] OnDetach() threw exception: ") + e.what());
		}
		catch (...)
		{
			RAY_CORE_ERROR(std::string("[LayerStack] OnDetach() threw unknown exception"));
		}

		std::unique_ptr<Layer> poppedLayer = std::move(m_Layers[index]);
		m_Layers.erase(m_Layers.begin() + index);

		if (index < m_LayerInsert)
		{
			m_LayerInsert--;
		}

		return poppedLayer;
	}

	std::unique_ptr<Layer> LayerStack::PopLayer(std::string_view name) noexcept
	{
		auto index = FindIndexByName(name);
		if (index == npos)
			return std::unique_ptr<Layer>();

		try
		{
			m_Layers[index]->OnDetach();
		}
		catch (const std::exception& e)
		{
			RAY_CORE_ERROR(std::string("[LayerStack] OnDetach() threw exception: ") + e.what());
		}
		catch (...)
		{
			RAY_CORE_ERROR(std::string("[LayerStack] OnDetach() threw unknown exception"));
		}

		std::unique_ptr<Layer> poppedLayer = std::move(m_Layers[index]);
		m_Layers.erase(m_Layers.begin() + index);

		if (index < m_LayerInsert)
		{
			m_LayerInsert--;
		}
		return poppedLayer;
	}

	bool LayerStack::Contains(const Layer* layer) const noexcept
	{
		return FindIndexByPointer(layer) != npos;
	}

	std::size_t LayerStack::Size() const noexcept
	{
		return m_Layers.size();
	}

	void LayerStack::Clear() noexcept
	{
		for (auto& layer : m_Layers)
		{
			if (!layer) continue;

			try
			{
				layer->OnDetach();
			}
			catch (const std::exception& e)
			{
				RAY_CORE_ERROR(std::string("[LayerStack] OnDetach() threw exception during Clear: ") + e.what());
			}
			catch (...)
			{
				RAY_CORE_ERROR(std::string("[LayerStack] OnDetach() threw unknown exception during Clear"));
			}
		}
		m_Layers.clear();
		m_LayerInsert = 0;
	}

	std::size_t LayerStack::FindIndexByPointer(const Layer* layer) const noexcept
	{
		for (std::size_t i = 0; i < m_Layers.size(); ++i)
		{
			if (m_Layers[i].get() == layer)
			{
				return i;
			}
		}
		return npos;
	}

	std::size_t LayerStack::FindIndexByName(std::string_view name) const noexcept
	{
		for (std::size_t i = 0; i < m_Layers.size(); ++i)
		{
			if (m_Layers[i]->GetName() == name)
			{
				return i;
			}
		}
		return npos;
	}

	void LayerStack::RollbackLayer(const Layer* rawLayer) noexcept
	{
		auto idx = FindIndexByPointer(rawLayer);
		if (idx != npos && idx < m_Layers.size())
		{
			m_Layers.erase(m_Layers.begin() + idx);
			if (idx < m_LayerInsert) 
				m_LayerInsert--;
		}
	}
}