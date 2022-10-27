project(Charcoal)

######### SOURCE FILES
file(GLOB_RECURSE SrcFiles
  "src/Charcoal/*.h"
  "src/Charcoal/*.cpp"
  "src/Platform/OpenGL/*.h"
  "src/Platform/OpenGL/*.cpp"

  "vendor/stb/*.h"
  "vendor/stb/*.cpp"
  )

set(SRC
  "src/Charcoal/Core/Application.cpp"
  "src/Charcoal/Core/Log.cpp"
  "src/Charcoal/Core/Utilities.cpp"

  "src/Charcoal/ImGui/ImGuiBuild.cpp"
  "src/Charcoal/ImGui/ImGuiLayer.cpp"
  "src/Charcoal/Layers/LayerStack.cpp"

  "src/Charcoal/Renderer/Buffer.cpp"
  "src/Charcoal/Renderer/Framebuffer.cpp"
  "src/Charcoal/Renderer/Material.cpp"
  "src/Charcoal/Renderer/Model.cpp"
  "src/Charcoal/Renderer/OrthographicCamera.cpp"
  "src/Charcoal/Renderer/PerspectiveCamera.cpp"
  "src/Charcoal/Renderer/Renderer2D.cpp"
  "src/Charcoal/Renderer/Renderer3D.cpp"
  "src/Charcoal/Renderer/RendererAPI.cpp"
  "src/Charcoal/Renderer/RendererCommand.cpp"
  "src/Charcoal/Renderer/ShaderManager.cpp"
  "src/Charcoal/Renderer/SkyBox.cpp"
  "src/Charcoal/Renderer/Texture.cpp"
  "src/Charcoal/Renderer/TextureUtilities.cpp"
  "src/Charcoal/Renderer/VertexArray.cpp"
  )

if (WIN32)
  file(GLOB_RECURSE WindowsSrcFiles "src/Platform/Windows/*.h" "src/PLatform/Windows/*.cpp")
  list(APPEND SrcFiles ${WindowsSrcFiles})
endif()
if (UNIX)
  file(GLOB_RECURSE LinuxSrcFiles "src/Platform/Linux/*.h" "src/Platform/Linux/*.cpp")
  list(APPEND SrcFiles ${LinuxSrcFiles})
endif()

add_library(Charcoal STATIC
  ${SrcFiles}
  )

#########

GetIncludeDir("GLFW" GLFW_dir)
GetIncludeDir("Glad" Glad_dir)
GetIncludeDir("ImGui" ImGui_dir)
GetIncludeDir("GLM" GLM_dir)
GetIncludeDir("stb" stb_dir)

target_include_directories(Charcoal PUBLIC
  "src"
  "../${GLFW_dir}"
  "../${Glad_dir}"
  "../${ImGui_dir}"
  "../${GLM_dir}"
  "../${stb_dir}"
  "vendor/spdlog/include"
  )

add_definitions(
  -DGLFW_INCLUDE_NONE
  -D_CRT_SECURE_NO_WARNINGS
  -DCH_ENABLE_ASSERTS
  #Due to fmt in spdlog using deprecated stuff:
  -D_SILENCE_CXX17_RESULT_OF_DEPRECATION_WARNING
  )

set_target_properties(Charcoal PROPERTIES LINKER_LANGUAGE CXX)

target_precompile_headers(Charcoal PRIVATE
  [["chpch.h"]]
  )

if (WIN32)
  #TODO: Make sure that the latest SDK is used.
  add_definitions(
	-DCH_PLATFORM_WINDOWS
	)
endif()
if (UNIX)
  add_definitions(
	-DCH_PLATFORM_LINUX
	)
endif()
