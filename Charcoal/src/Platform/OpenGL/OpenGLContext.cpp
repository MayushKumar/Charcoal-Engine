#include "chpch.h"
#include "OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Charcoal
{

	#define OPENGL_BREAK_ON_TRACE 0
	#define OPENGL_BREAK_ON_INFO 0
	#define OPENGL_BREAK_ON_WARN 0
	#define OPENGL_BREAK_ON_ERROR 0

	#define OPENGL_LOG_TRACE 0
	#define OPENGL_LOG_INFO 1
	#define OPENGL_LOG_WARN 1
	#define OPENGL_LOG_ERROR 1

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

		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback((GLDEBUGPROC)[](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
			switch(severity)
			{
				case GL_DEBUG_SEVERITY_NOTIFICATION:
                    #if OPENGL_LOG_TRACE
					CH_CORE_TRACE("OpenGL Debug Message {0}", message);
					#endif
					#if OPENGL_BREAK_ON_TRACE
					CH_CORE_ASSERT(false, "OpenGL Break On Trace")
					#endif
					break;
				case GL_DEBUG_SEVERITY_LOW:
				    #if OPENGL_LOG_INFO
					CH_CORE_INFO("OpenGL Debug Message {0}", message);
					#endif
					#if OPENGL_BREAK_ON_INFO
					CH_CORE_ASSERT(false, "OpenGL Break On Info")
					#endif
					break;
				case GL_DEBUG_SEVERITY_MEDIUM:
				    #if OPENGL_LOG_TRACE
					CH_CORE_WARN("OpenGL Debug Message {0}", message);
					#endif
					#if OPENGL_BREAK_ON_WARN
					CH_CORE_ASSERT(false, "OpenGL Break On Warn")
					#endif
					break;
				case GL_DEBUG_SEVERITY_HIGH:
				    #if OPENGL_LOG_ERROR
					CH_CORE_ERROR("OpenGL Debug Message {0}", message);
					#endif
					#if OPENGL_BREAK_ON_ERROR
					CH_CORE_ASSERT(false, "OpenGL Break On Error")
					#endif
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
