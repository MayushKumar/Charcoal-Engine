#include "Sandbox2D.h"
#include "Sandbox3D.h"

#include <Charcoal.h>
#include <Charcoal/EntryPoint/EntryPoint.h>

class SandboxApp : public Charcoal::Application
{

public:
	SandboxApp()
	{
		//PushLayer(new Sandbox2D());
		PushLayer(new Sandbox3D());
	}

	~SandboxApp() {}

};

//Application creation
Charcoal::Application* CreateApplication()
{
	Charcoal::Application* app = new SandboxApp();
	return app;
}