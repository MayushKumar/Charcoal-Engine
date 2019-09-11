#include "SandboxApp.h"

class ExampleLayer : public Charcoal::Layer
{

public:
	ExampleLayer() : Layer("Example Layer"){}

	void OnUpdate() override
	{
		
	}

	void OnEvent(Charcoal::Event& event) override
	{
		//CH_INFO(event);
	}
	
	void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello World!");
		ImGui::End();
	}

};

SandboxApp::SandboxApp()
{
	PushLayer(new ExampleLayer());
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