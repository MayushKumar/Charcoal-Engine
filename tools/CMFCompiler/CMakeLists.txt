project("CMFCompiler")

file(GLOB_RECURSE SrcFiles "src/*.h" "src/*.cpp")


add_executable(CMFCompiler
  ${SrcFiles}
  "vendor/tinyobjloader/tiny_obj_loader.h"
  "vendor/tinyobjloader/tiny_obj_loader.cc"
  "vendor/tinygltf/tiny_gltf.h"
  "vendor/tinygltf/tiny_gltf.cc"
  "../../Charcoal/vendor/stb/stb_image.h"
  )

GetIncludeDir("stb" stb_dir)
GetIncludeDir("GLM" GLM_dir)

target_include_directories(CMFCompiler PUBLIC
  "src"
  "../../${stb_dir}"
  "../../${GLM_dir}"
  "vendor/tinyobjloader"
  "vendor/tinygltf"
  )

set_target_properties(CMFCompiler PROPERTIES LINKER_LANGUAGE CXX)

if (WIN32)
  add_definitions(
	-DCH_PLATFORM_WINDOWS
	-D_CRT_SECURE_NO_WARNINGS
	)
endif()
if (UNIX)
  add_definitions(
	-DCH_PLATFORM_LINUX
	)
endif()
