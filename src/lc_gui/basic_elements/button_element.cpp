#include "lc_gui/basic_elements/button_element.hpp"

lc_gui::ButtonElement::ButtonElement(std::function<void(ButtonElement *)> clickCallback, const std::string &className, const std::string &clickClassStr)
    : Element(className), clickCallback(clickCallback), clickClassStr(clickClassStr) {}

bool lc_gui::ButtonElement::inputs(const glm::ivec2 &relativeCursorPosition)
{
    bool isWithinBorder = relativeCursorPosition.x >= 0 && relativeCursorPosition.y >= 0 && relativeCursorPosition.x < prevLayout.width && relativeCursorPosition.y < prevLayout.height;
    if (isWithinBorder)
        window->setCursor(GLFW_HAND_CURSOR);

    if (window->isLmbDown())
    {
        if (!lmbDown)
        {
            lmbDown = requireInputs = true;
            if (clickClassStr.size() > 0)
            {
                classList.add(clickClassStr);
                styleChange = true;
            }
        }
    }
    else if (lmbDown)
    {
        lmbDown = requireInputs = false;
        if (clickClassStr.size() > 0 && classList.contains(clickClassStr))
        {
            classList.remove(clickClassStr);
            styleChange = true;
        }

        if (isWithinBorder && clickCallback)
            clickCallback(this);
    }
    return requireInputs;
}

void lc_gui::ButtonElement::click()
{
    if (clickCallback)
        clickCallback(this);
}