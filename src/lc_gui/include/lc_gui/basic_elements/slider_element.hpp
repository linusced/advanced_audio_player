#pragma once

#include "../element.hpp"

namespace lc_gui
{
    class SliderElement : public Element
    {
    public:
        SliderElement(float min, float max, float value, std::function<void(SliderElement *, float &)> callback, const std::string &className, const std::string &mouseDownClassStr = "");

        void resizeMesh(const Style &style, const Layout &layout, const glm::ivec2 &windowSize) override;
        bool inputs(const glm::ivec2 &relativeCursorPosition) override;

        const float &getMin() const;
        const float &getMax() const;
        const float &getValue() const;

        void setMin(float min);
        void setMax(float max);
        void setValue(float value, bool call_callback = false);

        std::function<void(SliderElement *, float &)> callback;
        std::string mouseDownClassStr;

        bool callbackOnInput{false};

    private:
        float min, max, value;
    };
}