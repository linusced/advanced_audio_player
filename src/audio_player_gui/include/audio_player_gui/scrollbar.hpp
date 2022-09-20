#pragma once

#include "lc_gui/lc_gui.hpp"

namespace audio_player_gui
{
    class Scrollbar : public lc_gui::Element
    {
    public:
        Scrollbar(int max, int step, int height, int value, std::function<void(int)> callback, const std::string &className);

        void resizeMesh(const lc_gui::Style &style, const lc_gui::Layout &layout, const glm::ivec2 &windowSize) override;
        bool inputs(const glm::ivec2 &relativeCursorPosition) override;

        void setMax(int max);
        void setStep(int step);
        void setHeight(int height);
        void setValue(int value);

    private:
        int max, step, height, value;
        std::function<void(int)> callback;
    };
}