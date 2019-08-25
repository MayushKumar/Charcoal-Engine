#include "SandboxApp.h"

SandboxApp::SandboxApp()
{
	
}

SandboxApp::~SandboxApp()
{

}

//Application creation

Charcoal::Application* CreateApplication()
{
	Charcoal::Application* app = new SandboxApp();
	return app;
}