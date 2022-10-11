set(FILES
    scrollbar.cpp
    waveform/waveform.cpp waveform/inputs_zoom_and_panning.cpp waveform/inputs_set_cursor.cpp waveform/inputs_lmb_type.cpp waveform/inputs_lmb_down.cpp waveform/inputs_escape_key.cpp waveform/draw.cpp waveform/draw_utility.cpp)

add_library(audio_player_gui STATIC ${FILES})
target_link_libraries(audio_player_gui PUBLIC lc_gui advanced_audio_player)
target_include_directories(audio_player_gui PUBLIC include/)
