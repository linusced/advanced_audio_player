set(FILES
    window.cpp texture.cpp text_renderer.cpp shader.cpp lc_gui.cpp key_handler.cpp draw_loop.cpp data_loaders.cpp
    style/style.cpp style/style_layout.cpp
    renderer/renderer.cpp renderer/parse_css.cpp
    mesh/mesh_2d.cpp mesh/mesh_utility.cpp
    element/element.cpp element/resize.cpp element/inputs.cpp element/draw.cpp element/class_list.cpp element/container_mesh_border_radius.cpp element/container_mesh_border_width.cpp element/container_mesh_default.cpp
    basic_elements/button_element.cpp basic_elements/image_element.cpp basic_elements/slider_element.cpp basic_elements/text_element.cpp basic_elements/text_input_element.cpp)

add_library(lc_gui STATIC ${FILES})
target_link_libraries(lc_gui PUBLIC stb ${CONAN_LIBS_GLAD} ${CONAN_LIBS_GLFW} ${CONAN_LIBS_GLM})
target_include_directories(lc_gui PUBLIC include/)
