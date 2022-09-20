#include "audio_player_gui/scrollbar.hpp"

audio_player_gui::Scrollbar::Scrollbar(int max, int step, int height, int value, std::function<void(int)> callback, const std::string &className)
    : lc_gui::Element(className), max(max), step(step), height(height), value(value), callback(callback)
{
    mesh.vertices.resize(4);
    mesh.indices = {0, 1, 2, 0, 2, 3};
}

void audio_player_gui::Scrollbar::resizeMesh(const lc_gui::Style &style, const lc_gui::Layout &layout, const glm::ivec2 &windowSize)
{
    glm::vec4 color = style.colorProperties.at("color").isSet ? style.colorProperties.at("color").value : glm::vec4(1.0f);
    for (auto &v : mesh.vertices)
        v.color = color;

    int meshY = 0, meshHeight = 0;
    if (max != 0)
    {
        meshY = (value / (float)max) * layout.height;
        meshHeight = std::ceil((height / (float)max) * layout.height);
    }

    mesh.vertices[0].position = {layout.x, layout.y + meshY};
    mesh.vertices[1].position = {layout.x + layout.width, layout.y + meshY};
    mesh.vertices[2].position = {layout.x + layout.width, layout.y + meshY + meshHeight};
    mesh.vertices[3].position = {layout.x, layout.y + meshY + meshHeight};

    for (auto &v : mesh.vertices)
        v.position /= windowSize;

    mesh.applyVertexChange();
}
bool audio_player_gui::Scrollbar::inputs(const glm::ivec2 &relativeCursorPosition)
{
    if (max == 0)
        return false;

    window->setCursor(GLFW_HAND_CURSOR);

    int offset = (value / (float)max) * prevLayout.height,
        realHeight = std::ceil((height / (float)max) * prevLayout.height),
        newValue = value;

    if (window->isLmbDown())
    {
        if (!lmbDown)
        {
            lmbDown = requireInputs = true;
            lmbDownStart = relativeCursorPosition;
            lmbDownScrollMeshOffsetY = value;

            if (lmbDownStart.y < offset || lmbDownStart.y > offset + realHeight)
            {
                newValue = (relativeCursorPosition.y - realHeight / 2) / (float)prevLayout.height * max;
                lmbDownScrollMeshOffsetY = -1;
                styleChange = true;
            }
        }
        else
        {
            if (lmbDownScrollMeshOffsetY == -1)
                lmbDownScrollMeshOffsetY = value;

            newValue = (relativeCursorPosition.y - lmbDownStart.y) / (float)prevLayout.height * max + lmbDownScrollMeshOffsetY;
            styleChange = true;
        }
    }
    else if (lmbDown)
    {
        lmbDown = requireInputs = false;
        styleChange = true;
    }

    bool scroll = false;
    if (!lmbDown)
    {
        double mouseScrollY = window->getScrollY();
        if (mouseScrollY != 0.0)
        {
            newValue -= mouseScrollY * 33;
            styleChange = true;
            scroll = true;
        }
    }

    newValue = std::clamp(newValue, 0, max - height);

    if (newValue != value)
    {
        if (step == 0)
        {
            callback(newValue);
        }
        else
        {
            int prevStepValue = std::round(value / (float)step) * step,
                stepValue = std::round(newValue / (float)step) * step;

            if (stepValue != prevStepValue)
                callback(stepValue);
        }
        value = newValue;
    }
    return requireInputs || scroll;
}

void audio_player_gui::Scrollbar::setMax(int max)
{
    this->max = max;
    styleChange = true;
}
void audio_player_gui::Scrollbar::setStep(int step)
{
    this->step = step;
    styleChange = true;
}
void audio_player_gui::Scrollbar::setHeight(int height)
{
    this->height = height;
    styleChange = true;
}
void audio_player_gui::Scrollbar::setValue(int value)
{
    this->value = value;
    styleChange = true;
}