#include "lc_gui/draw_loop.hpp"

lc_gui::DrawLoop::DrawLoop(Window *window, const std::string &css)
    : window(window), guiRenderer(std::make_unique<lc_gui::Renderer>(css, window)) {}

void lc_gui::DrawLoop::loop()
{
    guiRenderer->resize();

    while (!window->shouldClose())
    {
        window->update();

        if (window->hasResized())
            guiRenderer->resize();

        guiRenderer->update();

        glClear(GL_COLOR_BUFFER_BIT);

        guiRenderer->draw();

        window->swapBuffers();
    }
}