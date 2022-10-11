cmake_minimum_required(VERSION 3.2)

project("Advanced Audio Player" VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include(FetchContent)

FetchContent_Declare(lc_gui GIT_REPOSITORY https://github.com/linusced/lc_gui GIT_TAG 1.0.4)
FetchContent_MakeAvailable(lc_gui)

FetchContent_Declare(lc_audio GIT_REPOSITORY https://github.com/linusced/lc_audio GIT_TAG 1.0.0)
FetchContent_MakeAvailable(lc_audio)

FetchContent_Declare(lc_utility GIT_REPOSITORY https://github.com/linusced/lc_utility GIT_TAG 0.2.2)
FetchContent_MakeAvailable(lc_utility)

add_subdirectory(config)
add_subdirectory(src)

set(OUT a.out) 
set(LINK_LIBS draw_loops)
add_subdirectory(app)
 