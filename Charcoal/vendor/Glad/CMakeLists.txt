project(Glad)

set(SrcFiles
  "include/glad/glad.h"
  "include/KHR/khrplatform.h"
  "src/glad.c"
  )

add_library(Glad
  ${SrcFiles}
  )

target_include_directories(Glad PUBLIC
  "include"
  )

set_target_properties(Glad PROPERTIES LINKER_LANGUAGE CXX)
