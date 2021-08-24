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
