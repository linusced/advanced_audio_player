set(FILES
    update_buffer_data.cpp thread_funcs.cpp file_setters.cpp file_getters.cpp custom_file.cpp advanced_audio_player.cpp)

add_library(advanced_audio_player STATIC ${FILES})
target_link_libraries(advanced_audio_player lc_audio)
target_link_libraries(advanced_audio_player lc_utility)
target_include_directories(advanced_audio_player PUBLIC include/)
