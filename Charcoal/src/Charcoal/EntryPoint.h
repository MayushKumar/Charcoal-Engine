#pragma once

#include "Charcoal/Application.h"

namespace Charcoal
{
	Application* CreatApplication();

	int main()
	{
		Application* app = CreatApplication();
		app->Run();
	}
}