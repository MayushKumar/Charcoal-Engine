#include "SandboxApp.h"

SandboxApp::SandboxApp()
{
	m_Window = Charcoal::Window::Create();
}

SandboxApp::~SandboxApp()
{

}

void SandboxApp::Run()
{
	CH_INFO("Sandbox Ran...");
	while (true)
	{
		m_Window->OnUpdate();
	}
}

//Application creation

Charcoal::Application* CreateApplication()
{
	Charcoal::Application* app = new SandboxApp();
	return app;
}