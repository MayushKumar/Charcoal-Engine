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
		CH_CORE_INFO("Graphics Vendor: {0}\n\t\t\tRenderer: {1}\n\t\t\tOpenGL Version: {2}\n\t\t\tGLSL Version: {3}", glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}