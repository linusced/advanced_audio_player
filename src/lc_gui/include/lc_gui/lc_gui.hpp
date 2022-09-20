#pragma once

#include "data_loaders.hpp"
#include "draw_loop.hpp"
#include "basic_elements/text_element.hpp"
#include "basic_elements/button_element.hpp"
#include "basic_elements/image_element.hpp"
#include "basic_elements/slider_element.hpp"
#include "basic_elements/text_input_element.hpp"
#include "key_handler.hpp"

namespace lc_gui
{
    void init(bool multisample);
    void terminate();

    void enableBlend();
}