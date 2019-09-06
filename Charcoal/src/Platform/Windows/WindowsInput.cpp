#include "chpch.h"
#include "WindowsInput.h"

#include "GLFW/glfw3.h"

#include "Charcoal/Application.h"

namespace Charcoal
{

	Input* Input::s_Instance = new WindowsInput();

	inline bool WindowsInput::IsKeyPressedImpl(int keyCode)
	{
		auto window = static_cast<GLFWwindow*>(Application::GetApplication().GetWindow().GetNativeWindow());
		return glfwGetKey(window, keyCode);
	}

	inline bool WindowsInput::IsMouseButtonPressedImpl(int mouseCode)
	{
		auto window = static_cast<GLFWwindow*>(Application::GetApplication().GetWindow().GetNativeWindow());
		return glfwGetMouseButton(window, mouseCode);
	}

	inline std::pair<float, float> WindowsInput::GetMousePosImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::GetApplication().GetWindow().GetNativeWindow());
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		return std::make_pair((float)xPos, (float)yPos);
	}

	inline float WindowsInput::GetMouseXImpl()
	{
		auto [x, y] = GetMousePosImpl();
		return x;
	}

	inline float WindowsInput::GetMouseYImpl()
	{
		auto [x, y] = GetMousePosImpl();
		return y;
	}

}
