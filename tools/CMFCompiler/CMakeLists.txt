project("CMFCompiler")

file(GLOB_RECURSE SrcFiles "src/*.h" "src/*.cpp")


add_executable(CMFCompiler
  ${SrcFiles}
  "vendor/tinyobjloader/tiny_obj_loader.h"
  "vendor/tinyobjloader/tiny_obj_loader.cc"
  "../../Charcoal/vendor/stb_image/stb_image.h"
  )

GetIncludeDir("stb_image" stb_image_dir)
GetIncludeDir("GLM" GLM_dir)

target_include_directories(CMFCompiler PUBLIC
  "src"
  "../../${stb_image_dir}"
  "../../${GLM_dir}"
  "vendor/tinyobjloader"
  )

set_target_properties(CMFCompiler PROPERTIES LINKER_LANGUAGE CXX)

if (WIN32)
  #TODO: Make sure that the latest SDK is used.
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
