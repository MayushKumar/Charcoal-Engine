project(CharcoalEditor)

file(GLOB_RECURSE SrcFiles "src/*.h" "src/*.cpp")

add_executable(CharcoalEditor
  ${SrcFiles}
  )

GetIncludeDir("ImGui" ImGui_dir)
GetIncludeDir("GLM" GLM_dir)
GetIncludeDir("spdlog" spdlog_dir)

target_include_directories(CharcoalEditor PUBLIC
  "src"
  "../../${ImGui_dir}"
  "../../${GLM_dir}"
  "../../${spdlog_dir}"
  )

target_link_libraries(CharcoalEditor
  Charcoal
  GLFW
  Glad
  ImGui
  )

set_target_properties(CharcoalEditor PROPERTIES LINKER_LANGUAGE CXX)

add_definitions(
  -D_SILENCE_CXX17_RESULT_OF_DEPRECATION_WARNING  
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
  target_link_options(CharcoalEditor PRIVATE
	-lpthread
	)
endif()
