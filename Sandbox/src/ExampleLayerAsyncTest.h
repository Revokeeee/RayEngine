
#pragma once

#include "RayEngine/Core/Layer.h"
#include "RayEngine/Core/Application.h"

#include <memory>
#include <iostream>

class ExampleChildLayer : public RayEngine::Layer
{
public:
    ExampleChildLayer() : RayEngine::Layer("ExampleChildAsync") {}
    void OnAttach() override { RAY_CLIENT_INFO("ExampleChildAsync attached"); }
    void OnDetach() override { RAY_CLIENT_INFO("ExampleChildAsync detached"); }
    void OnUpdate(float) override { RAY_CLIENT_INFO("ExampleChildAsync OnUpdate"); }
};

class ExampleLayerAsync : public RayEngine::Layer
{
public:
    ExampleLayerAsync()
        : RayEngine::Layer("ExampleAsync")
        , m_childPtr(nullptr)
        , m_pushed(false)
        , m_popRequested(false)
    {
    }

    void OnAttach() override { RAY_CLIENT_INFO("ExampleAsync attached"); }
    void OnDetach() override { RAY_CLIENT_INFO("ExampleAsync detached"); }

    void OnUpdate(float) override
    {
        // No switch — use simple boolean guards to run each step once.
        if (!m_pushed)
        {
            auto child = std::make_unique<ExampleChildLayer>();
            m_childPtr = child.get(); // keep non-owning raw pointer to pop later
            RAY_CLIENT_INFO("ExampleAsync: Enqueue PushLayerAsync(child='{}')", std::string(m_childPtr->GetName()));
            RayEngine::Application::GetInstance().PushLayerAsync(std::move(child));
            m_pushed = true;
            return;
        }

        if (m_pushed && !m_popRequested)
        {
            if (m_childPtr)
            {
                RAY_CLIENT_INFO("ExampleAsync: Enqueue PopLayerAsync(child='{}')", std::string(m_childPtr->GetName()));
                RayEngine::Application::GetInstance().PopLayerAsync(m_childPtr, [](std::unique_ptr<RayEngine::Layer> popped) {
                    if (popped)
                        RAY_CLIENT_INFO("ExampleAsync Pop callback: popped '{}'", std::string(popped->GetName()));
                    else
                        RAY_CLIENT_INFO("ExampleAsync Pop callback: nullptr");
                    });
            }
            m_popRequested = true;
            return;
        }

        if (m_pushed && m_popRequested)
        {
            RAY_CLIENT_INFO("ExampleAsync: operations enqueued. Press Enter to continue and see logs...");
            std::cin.get(); // block so you can read the logs in console
            // Prevent repeated blocking if OnUpdate continues to be called.
            m_pushed = false;
            m_popRequested = false;
        }
    }

private:
    ExampleChildLayer* m_childPtr; // non-owning
    bool m_pushed;
    bool m_popRequested;
};