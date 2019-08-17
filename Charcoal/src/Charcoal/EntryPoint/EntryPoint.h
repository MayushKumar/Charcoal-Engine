#pragma once

//#include "Charcoal/Application.h"

Charcoal::Application* CreateApplication();

int main()
{
	Charcoal::Log::Init();
	Charcoal::Application* app = CreateApplication();
	app->Run();
	delete app;
	return 0;
}