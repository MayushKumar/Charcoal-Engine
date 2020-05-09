#include "chpch.h"
#include "OpenGLContext.h"

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
		CH_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		CH_CORE_ASSERT(status, "Failed to initialize Glad");
		CH_CORE_INFO("Graphics Vendor: {0}\n\t\t\tRenderer: {1}\n\t\t\tOpenGL Version: {2}\n\t\t\tGLSL Version: {3}", glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
		
		glFrontFace(GL_CCW);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);

		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback((GLDEBUGPROC)[](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
			switch(severity)
			{
				case GL_DEBUG_SEVERITY_NOTIFICATION:
					CH_CORE_TRACE("OpenGL Debug Message {0}", message);
					break;
				case GL_DEBUG_SEVERITY_LOW:
					CH_CORE_INFO("OpenGL Debug Message {0}", message);
					break;
				case GL_DEBUG_SEVERITY_MEDIUM:
					CH_CORE_WARN("OpenGL Debug Message {0}", message);
					break;
				case GL_DEBUG_SEVERITY_HIGH:
					CH_CORE_ERROR("OpenGL Debug Message {0}", message);
					break;
			}
			}, nullptr);
	}

	void OpenGLContext::SwapBuffers()
	{
		CH_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}

}
