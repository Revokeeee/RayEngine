#pragma once
#include <RayEngine.h>

using namespace RayEngine;

class ExampleLayer : public Layer
{
public:
	ExampleLayer() : Layer("Example") {}
	void OnAttach() override { RAY_CLIENT_INFO("ExampleLayer attached"); }
	void OnDetach() override { RAY_CLIENT_INFO("ExampleLayer detached"); }
	void OnUpdate(float dt) override 
	{
		
		RAY_CLIENT_INFO("Application is running");

		// Simulate work by sleeping for a short duration
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
};
