#pragma once

#include "Charcoal/Core/Application.h"

Charcoal::Application* CreateApplication();

int main()
{
	Charcoal::Log::Init();
	Charcoal::Application* app = CreateApplication();
	app->Run();
	delete app;
	CH_PROFILE_END_SESSION();
	return 0;
}
