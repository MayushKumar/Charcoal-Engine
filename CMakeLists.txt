cmake_minimum_required(VERSION 3.15)

project("Charcoal Engine")

SET (CMAKE_CXX_STANDARD 17)


if(UNIX)
  SET (CMAKE_EXE_LINKER_FLAGS      "-Wl,--no-as-needed -fuse-ld=lld -ldl")
  SET (CMAKE_CXX_FLAGS_DEBUG       "${CMAKE_CXX_FLAGS_DEBUG} -fstandalone-debug")
endif()

set(IncludeDirs
  "Charcoal/vendor/GLFW/include"
  "Charcoal/vendor/Glad/include"
  "Charcoal/vendor/ImGui"
  "Charcoal/vendor/GLM"
  "Charcoal/vendor/stb"
  "Charcoal/vendor/spdlog"
  )

set(IncludeProjs
  "GLFW"
  "Glad"
  "ImGui"
  "GLM"
  "stb"
  "spdlog"
  )

function(GetIncludeDir projname out)
  list(FIND IncludeProjs ${projname} i)
  list(GET IncludeDirs ${i} result)
  set(${out} ${result} PARENT_SCOPE)
endfunction()

# Forcing colored outputs(mainly applies to Ninja builds)
option (FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." TRUE)
if (${FORCE_COLORED_OUTPUT})
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
       add_compile_options (-fdiagnostics-color=always)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
       add_compile_options (-fcolor-diagnostics)
    endif ()
endif ()

add_subdirectory(Charcoal)
add_subdirectory(Sandbox)
add_subdirectory(tools/CharcoalEditor)
add_subdirectory(tools/CMFCompiler)
add_subdirectory(Charcoal/vendor/Glad)
add_subdirectory(Charcoal/vendor/GLFW)
add_subdirectory(Charcoal/vendor/ImGui)
