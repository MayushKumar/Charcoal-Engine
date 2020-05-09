#pragma once
#include <chpch.h>

#include "Charcoal/Events/Event.h"

namespace Charcoal
{

	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(std::string title = "Charcoal Engine", unsigned int width = 1280, unsigned int height = 720) 
			: Title(title), Width(width), Height(height) {}
	};

	enum class WindowCursorMode
	{
		Normal = 0, HideAndLock, Hide
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual inline unsigned int GetWidth() const = 0;
		virtual inline unsigned int GetHeight() const = 0;
		virtual inline void* GetNativeWindow() const = 0;

		virtual void SetVSync(bool enabled) = 0;
		virtual inline bool IsVSync() const = 0;

		virtual void SetCursorMode(WindowCursorMode mode) = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		static Window* Create(const WindowProps& props = WindowProps());
	};

}