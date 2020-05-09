#include "chpch.h"
#include "WindowsWindow.h"

#include "Charcoal/Events/Event.h"
#include "Charcoal/Events/KeyEvent.h"
#include "Charcoal/Events/MouseEvent.h"
#include "Charcoal/Events/ApplicationEvent.h"

#include "Platform/OpenGL/OpenGLContext.h"

#include <GLFW/glfw3.h>

namespace Charcoal
{

	static bool s_GLFWInitialized = false;

	Window* Window::Create(const WindowProps& props)
	{
		CH_PROFILE_FUNCTION();

		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		ShutDown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		CH_PROFILE_FUNCTION();

		m_Data.Width = props.Width;
		m_Data.Height = props.Height;
		m_Data.Title = props.Title;

		CH_CORE_INFO("Creating Window: Width = {0}, Height = {1}, Title = {2}", m_Data.Width, m_Data.Height, m_Data.Title);

		if (!s_GLFWInitialized) 
		{
			int status = glfwInit();
			CH_CORE_ASSERT(status, "Failed To Create Window!");
			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);

		m_Context = new OpenGLContext(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		//GLFW callbacks
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowClosedEvent event;
			data.EventCallback(event);
			});

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;
			WindowResizeEvent event = WindowResizeEvent(width, height);
			data.EventCallback(event);
			});
	
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			
			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event = KeyPressedEvent(key, false);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event = KeyReleasedEvent(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event = KeyPressedEvent(key, true);
					data.EventCallback(event);
					break;
				}
			}
			});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int character) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			KeyTypedEvent event = KeyTypedEvent(character);
			data.EventCallback(event);
			});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event = MouseButtonPressedEvent(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event = MouseButtonReleasedEvent(button);
					data.EventCallback(event);
					break;
				}
			}
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseScrolledEvent event = MouseScrolledEvent((float)xOffset, (float)yOffset);
			data.EventCallback(event);
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseMovedEvent event = MouseMovedEvent((float)xPos, (float)yPos);
			data.EventCallback(event);
			});
	}

	void WindowsWindow::ShutDown()
	{
		CH_PROFILE_FUNCTION();

		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate()
	{
		CH_PROFILE_FUNCTION();

		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
		m_Data.VSync = enabled;
	}

	void WindowsWindow::SetCursorMode(WindowCursorMode mode)
	{
		switch (mode)
		{
		case WindowCursorMode::Normal:
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			break;
		case WindowCursorMode::Hide:
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			break;
		case WindowCursorMode::HideAndLock:
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			break;
		default:
			CH_CORE_ASSERT(false, "Invalid Window Cursor Mode");
			break;
		}
	}

}
