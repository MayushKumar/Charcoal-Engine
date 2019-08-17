#include "SandboxApp.h"

SandboxApp::SandboxApp()
{

}

SandboxApp::~SandboxApp()
{

}

void SandboxApp::Run()
{
	Charcoal::MouseButtonPressedEvent event = Charcoal::MouseButtonPressedEvent(2);
	CH_INFO(event);
	CH_INFO("Sandbox Ran...");
}

//Application creation

Charcoal::Application* CreateApplication()
{
	Charcoal::Application* app = new SandboxApp();
	return app;
}