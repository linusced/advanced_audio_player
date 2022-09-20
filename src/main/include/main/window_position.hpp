#pragma once

#include "lc_gui/window.hpp"
#include "lc_gui/style.hpp"

namespace window_position
{
    void load(const std::string &appSupportFilePath, lc_gui::Layout *windowLayout);
    void save(const std::string &appSupportFilePath, lc_gui::Window *window);
};