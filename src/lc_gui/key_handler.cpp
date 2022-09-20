#include "lc_gui/key_handler.hpp"

const double lc_gui::KeyHandler::KEY_DOWN_PRESS_TIME{0.5};

lc_gui::KeyHandler::KeyHandler(Window *window)
    : window(window) {}

void lc_gui::KeyHandler::addKey(std::vector<GLenum> keyCombination, bool allowKeyPress, std::function<void(int)> callback, int id)
{
    keys.push_back(Key());
    auto &k = keys.back();
    k.keyCombination = keyCombination;
    k.allowKeyPress = allowKeyPress;
    k.id = id;
    k.callback = callback;
}

void lc_gui::KeyHandler::update()
{
    for (auto &k : keys)
    {
        bool keyCombinationActive{true};

        for (auto &c : k.keyCombination)
            if (!glfwGetKey(*window, c))
            {
                keyCombinationActive = false;
                break;
            }

        if (keyCombinationActive)
        {
            if (!k.keyDown)
            {
                k.keyDown = true;
                k.keyDownTime = window->getCurrentTime();
                k.callback(k.id);
            }
            else if (k.allowKeyPress && window->getCurrentTime() - k.keyDownTime > KEY_DOWN_PRESS_TIME)
                k.callback(k.id);
        }
        else
            k.keyDown = false;
    }
}