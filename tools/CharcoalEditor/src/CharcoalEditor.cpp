#include <Charcoal.h>
#include <Charcoal/EntryPoint/EntryPoint.h>

#include "EditorLayer.h"

class CharcoalEditor : public Charcoal::Application
{

public:
	CharcoalEditor()
	{
		PushLayer(new EditorLayer());
	}

	~CharcoalEditor() {}

};

//Application creation
Charcoal::Application* CreateApplication()
{
	Charcoal::Application* app = new CharcoalEditor();
	return app;
}
