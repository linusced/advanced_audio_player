cmake_minimum_required(VERSION 3.16)

project("Advanced Audio Player" VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_subdirectory(config)
add_subdirectory(external)
add_subdirectory(src)

set(OUT a.out)
set(LINK_LIBS draw_loops main
     "-framework IOKit" "-framework Cocoa" "-framework CoreVideo" "-framework OpenGL" "-framework OpenAL")

add_subdirectory(app)
