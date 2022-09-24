set(FILES
    update.cpp topbar_gui.cpp sidebar_gui.cpp file_explorer.cpp directories.cpp callbacks.cpp btmbar_gui.cpp)

add_library(file_explorer STATIC ${FILES})
target_link_libraries(file_explorer lc_gui)
target_include_directories(file_explorer PUBLIC include/)
 