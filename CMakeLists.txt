cmake_minimum_required(VERSION 3.15)

project("Charcoal Engine")

# SET (CMAKE_C_COMPILER             "/usr/bin/clang")
# SET (CMAKE_C_FLAGS                "-Wall -std=c99")
# SET (CMAKE_C_FLAGS_DEBUG          "-g")
# SET (CMAKE_C_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
# SET (CMAKE_C_FLAGS_RELEASE        "-O4 -DNDEBUG")
# SET (CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g")

# SET (CMAKE_CXX_COMPILER             "/usr/bin/clang++")
# SET (CMAKE_CXX_FLAGS                "-Wall")
# SET (CMAKE_CXX_FLAGS_DEBUG          "-g")
# SET (CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
# SET (CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
# SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

# SET (CMAKE_AR      "llvm-ar")
# SET (CMAKE_LINKER  "llvm-ld")
# SET (CMAKE_NM      "llvm-nm")
# SET (CMAKE_OBJDUMP "llvm-objdump")
# SET (CMAKE_RANLIB  "llvm-ranlib")

SET (CMAKE_CXX_STANDARD 17)

if(UNIX)
  SET (CMAKE_EXE_LINKER_FLAGS      "-Wl,--no-as-needed -ldl")
endif()

set(IncludeDirs
  "Charcoal/vendor/GLFW/include"
  "Charcoal/vendor/Glad/include"
  "Charcoal/vendor/ImGui"
  "Charcoal/vendor/GLM"
  "Charcoal/vendor/stb_image"
  "Charcoal/vendor/spdlog"
  )

set(IncludeProjs
  "GLFW"
  "Glad"
  "ImGui"
  "GLM"
  "stb_image"
  "spdlog"
  )

function(GetIncludeDir projname out)
  list(FIND IncludeProjs ${projname} i)
  list(GET IncludeDirs ${i} result)
  set(${out} ${result} PARENT_SCOPE)
endfunction()

add_subdirectory(Charcoal)
add_subdirectory(Sandbox)
add_subdirectory(tools/CharcoalEditor)
add_subdirectory(tools/CMFCompiler)
add_subdirectory(Charcoal/vendor/Glad)
add_subdirectory(Charcoal/vendor/GLFW)
add_subdirectory(Charcoal/vendor/ImGui)
