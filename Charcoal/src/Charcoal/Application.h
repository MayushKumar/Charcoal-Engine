#pragma once

namespace Charcoal {

	class Application
	{

	public:
		Application();
		virtual ~Application();
	public:
		virtual void Run() = 0;

	};

}