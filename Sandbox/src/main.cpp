#include<iostream>
#include "RayEngine.h"

int main()
{
	auto& app = RayEngine::Application::GetInstance();
	if (app.Run())
	{
		return 0;
	}
	else
	{
		return -1;
	}
}