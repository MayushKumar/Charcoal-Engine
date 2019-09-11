#pragma once

#include "Charcoal/Window.h"

#include "Charcoal/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Charcoal
{

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }
		inline void* GetNativeWindow() const override { return m_Window; }
		void SetVSync(bool enabled) override;
		inline bool IsVSync() const override { return m_Data.VSync; }

		void SetEventCallback(const EventCallbackFn& eventCallback) override { m_Data.EventCallback = eventCallback; }

	private:
		void Init(const WindowProps& props);
		void ShutDown();

	private:
		GLFWwindow* m_Window;
		GraphicsContext* m_Context;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};
		WindowData m_Data;
		
	};

}