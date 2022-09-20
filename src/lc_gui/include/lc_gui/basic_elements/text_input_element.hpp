#pragma once

#include "text_element.hpp"

namespace lc_gui
{
    class TextInputElement : public TextElement
    {
    public:
        TextInputElement(TextRenderer::Font *font, std::string text, const std::string &className, std::function<void(TextInputElement *, std::string &)> callback = nullptr);

        bool inputs(const glm::ivec2 &relativeCursorPosition) override;

        std::function<void(TextInputElement *, std::string &)> callback;
        bool onlyCallbackOnEnterPress{false}, resetTextOnEscape{false};

    private:
        bool arrowLeftKeyDown{false}, arrowRightKeyDown{false}, backspaceKeyDown{false};
        double backspaceKeyDownTime{0.0}, arrowLeftKeyDownTime{0.0}, arrowRightKeyDownTime{0.0}, prevChangeTime{0.0};
        unsigned int textCursorOffset{0};
        std::string prevText;
    };
}