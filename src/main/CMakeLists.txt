add_library(main STATIC files.cpp window_position.cpp)
target_link_libraries(main lc_gui)
target_include_directories(main PUBLIC include/)
