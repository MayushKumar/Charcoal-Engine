#pragma once

#include <Charcoal.h>
class SandboxApp : public Charcoal::Application
{

public:
	SandboxApp();
	~SandboxApp();

public:
	void Run();

private:
	Charcoal::Window* m_Window;
};