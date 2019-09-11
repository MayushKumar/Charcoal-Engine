#include "chpch.h"
#include "OpenGLContext.h"

#include "Charcoal/Core.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Charcoal
{

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
	{
		CH_CORE_ASSERT(m_WindowHandle, "Window Handle is null!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		CH_CORE_ASSERT(status, "Failed to initialize Glad");
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}