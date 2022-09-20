cmake_minimum_required(VERSION 3.16)

project(AdvancedAudioPlayer VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenGL REQUIRED)
find_package(OpenAL REQUIRED)
find_package(glfw3 REQUIRED)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")
include(AddGitSubmodule)

add_git_submodule(external/libsamplerate)

add_subdirectory(config)
add_subdirectory(src)

set(EXEC a.out)
set(LIBS glfw samplerate extra draw_loops main
    "-framework IOKit" "-framework Cocoa" "-framework CoreVideo" "-framework OpenGL" "-framework OpenAL")

add_subdirectory(app)
