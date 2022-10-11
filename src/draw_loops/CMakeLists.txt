set(FILES
    menu_loop.cpp menu_loop_update_projects.cpp menu_loop_update_new_project_files.cpp
    main_loop/while_loop.cpp main_loop/waveform_lmb_type_gui.cpp main_loop/waveform_data_callbacks.cpp main_loop/waveform_callbacks.cpp main_loop/update_waveform_data.cpp main_loop/update_files_and_file_manager_callback.cpp main_loop/update_file_manager.cpp main_loop/main_loop.cpp main_loop/main_controls_gui.cpp main_loop/loading_gui.cpp main_loop/keyframe_editor_gui.cpp main_loop/keyframe_editor_callbacks.cpp main_loop/frame_progress.cpp main_loop/create_key_handler.cpp main_loop/callbacks.cpp)

add_library(draw_loops STATIC ${FILES})
target_link_libraries(draw_loops PUBLIC audio_player_gui file_explorer)
target_include_directories(draw_loops PUBLIC include/)
