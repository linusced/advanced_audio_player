set(FILES
    source.cpp filters.cpp file.cpp engine.cpp engine_thread.cpp buffer.cpp beats.cpp)

add_library(lc_audio STATIC ${FILES})
target_include_directories(lc_audio PUBLIC include/ /usr/local/include)
