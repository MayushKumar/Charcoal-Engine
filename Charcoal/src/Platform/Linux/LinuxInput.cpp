#include "chpch.h"
#include "LinuxInput.h"

#include "GLFW/glfw3.h"

#include "Charcoal/Core/Application.h"

namespace Charcoal
{

	Input* Input::s_Instance = new LinuxInput();

	inline bool LinuxInput::IsKeyPressedImpl(int keyCode)
	{
		auto window = static_cast<GLFWwindow*>(Application::GetApplication().GetWindow().GetNativeWindow());
		return glfwGetKey(window, keyCode);
	}

	inline bool LinuxInput::IsMouseButtonPressedImpl(int mouseCode)
	{
		auto window = static_cast<GLFWwindow*>(Application::GetApplication().GetWindow().GetNativeWindow());
		return glfwGetMouseButton(window, mouseCode);
	}

	inline std::pair<float, float> LinuxInput::GetMousePosImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::GetApplication().GetWindow().GetNativeWindow());
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		return std::make_pair((float)xPos, (float)yPos);
	}

	inline float LinuxInput::GetMouseXImpl()
	{
		auto [x, y] = GetMousePosImpl();
		return x;
	}

	inline float LinuxInput::GetMouseYImpl()
	{
		auto [x, y] = GetMousePosImpl();
		return y;
	}

}
