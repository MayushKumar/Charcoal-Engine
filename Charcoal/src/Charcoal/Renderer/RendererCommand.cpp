#include <chpch.h>
#include "RendererCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Charcoal
{

	RendererAPI* RendererCommand::s_RendererAPI = new OpenGLRendererAPI();

}