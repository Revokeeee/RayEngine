#include<iostream>

#include "ExampleLayer.h"
#include "ExampleLayerDirectTest.h"
#include "ExampleLayerAsyncTest.h"

#include "RayEngine.h"

int main()
{
	auto& app = RayEngine::Application::GetInstance();

	if (!app.Initialize()) // explicit init with error reporting
		return -1;

	app.PushLayer(std::make_unique<ExampleLayerAsync>());
	return app.Run() ? 0 : -1;
}