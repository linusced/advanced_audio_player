#pragma once

#include "renderer.hpp"

namespace lc_gui
{
    class DrawLoop
    {
    public:
        DrawLoop(Window *window, const std::string &css);
        virtual ~DrawLoop() {}
        virtual void loop();

    protected:
        Window *window;
        std::unique_ptr<Renderer> guiRenderer;
    };
}