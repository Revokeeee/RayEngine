
#pragma once

#include "RayEngine/Core/Layer.h"
#include "RayEngine/Core/Application.h"

#include <memory>
#include <iostream>

class ExampleChildLayerDirect : public RayEngine::Layer
{
public:
    ExampleChildLayerDirect() : RayEngine::Layer("ExampleChildDirect") {}
    void OnAttach() override { RAY_CLIENT_INFO("ExampleChildDirect attached"); }
    void OnDetach() override { RAY_CLIENT_INFO("ExampleChildDirect detached"); }
    void OnUpdate(float) override { RAY_CLIENT_INFO("ExampleChildDirect OnUpdate"); }
};

class ExampleLayerDirect : public RayEngine::Layer
{
public:
    ExampleLayerDirect()
        : RayEngine::Layer("ExampleDirect")
        , m_childPtr(nullptr)
        , m_pushed(false)
        , m_popped(false)
    {
    }

    void OnAttach() override { RAY_CLIENT_INFO("ExampleDirect attached"); }
    void OnDetach() override { RAY_CLIENT_INFO("ExampleDirect detached"); }

    void OnUpdate(float) override
    {
        // Single-threaded direct mutation test (no async).
        if (!m_pushed)
        {
            auto child = std::make_unique<ExampleChildLayerDirect>();
            m_childPtr = child.get();
            RAY_CLIENT_INFO("ExampleDirect: PushLayer(child='{}')", std::string(m_childPtr->GetName()));
            // Direct synchronous push
            RayEngine::Application::GetInstance().GetLayerStack().PushLayer(std::move(child));
            m_pushed = true;
            return;
        }

        if (m_pushed && !m_popped)
        {
            if (m_childPtr)
            {
                RAY_CLIENT_INFO("ExampleDirect: PopLayer(child='{}')", std::string(m_childPtr->GetName()));
                // Direct synchronous pop; returns ownership to this frame
                auto popped = RayEngine::Application::GetInstance().GetLayerStack().PopLayer(m_childPtr);
                if (popped)
                    RAY_CLIENT_INFO("ExampleDirect Pop result: popped '{}'", std::string(popped->GetName()));
                else
                    RAY_CLIENT_INFO("ExampleDirect Pop result: nullptr");
            }
            m_popped = true;
            return;
        }

        if (m_pushed && m_popped)
        {
            RAY_CLIENT_INFO("ExampleDirect: operations complete. Press Enter to continue and see logs...");
            std::cin.get(); // block so you can read console output
            // prevent repeated blocking
            m_pushed = false;
            m_popped = false;
        }
    }

private:
    ExampleChildLayerDirect* m_childPtr; // non-owning
    bool m_pushed;
    bool m_popped;
};