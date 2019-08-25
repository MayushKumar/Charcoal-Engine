#pragma once

#include <Charcoal.h>

#define BIND_EVENT_FUNC(x) std::bind(&SandboxApp::x, this, std::placeholders::_1);

class SandboxApp : public Charcoal::Application
{

public:
	SandboxApp();
	~SandboxApp();

};