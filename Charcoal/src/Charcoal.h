#pragma once

//For client apps only!

#include <imgui.h>

#include <Charcoal/Core/Core.h>
#include <Charcoal/Core/Log.h>
#include <Charcoal/Core/Application.h>
#include <Charcoal/Events/Event.h>
#include <Charcoal/Layers/Layer.h>

#include <Charcoal/Core/Timestep.h>

#include <Charcoal/ImGui/ImGuiLayer.h>

#include <Charcoal/Input/Input.h>
#include <Charcoal/Input/KeyCodes.h>
#include <Charcoal/Input/MouseCodes.h>

#include <Charcoal/Renderer/Renderer.h>
#include <Charcoal/Renderer/RendererCommand.h>
#include <Charcoal/Renderer/Buffer.h>
#include <Charcoal/Renderer/Shader.h>
#include <Charcoal/Renderer/VertexArray.h>
#include <Charcoal/Renderer/Texture.h>
#include <Charcoal/Renderer/RendererEnums.h>
#include <Charcoal/Renderer/Model.h>

#include <Charcoal/Utility/OrthographicCameraController.h>
#include <Charcoal/Utility/PerspectiveCameraController.h>

#include <Charcoal/Debug/Instrumentor.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>