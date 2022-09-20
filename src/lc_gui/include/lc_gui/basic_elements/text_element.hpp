#pragma once

#include "../element.hpp"
#include "../text_renderer.hpp"

#include <mutex>

namespace lc_gui
{
    class TextElement : public Element
    {
    public:
        TextElement(TextRenderer::Font *font, std::string text, const std::string &className, bool allowLineWrap, bool continuousThread, bool shrinkOnOverflow = true);
        ~TextElement();

        void resizeMesh(const Style &style, const Layout &layout, const glm::ivec2 &windowSize) override;
        void update() override;

        const std::string &getText() const;
        void setText(std::string text);

        bool threadIsActive() const;
        void stopThread();

        const int &getLineLayoutY(int lineIndex) const;
        const int &getLineLayoutHeight(int lineIndex) const;

    protected:
        static void renderText(TextElement *instance);
        static void continuousThread_func(TextElement *instance);

        void updateCursorPos(int cursorPos);

        TextRenderer::Font *font;
        std::unique_ptr<Texture> textImg;
        std::string text;

        TextRenderer textRenderer;
        std::vector<unsigned char> outputTextureBytes;
        int cursorPos{-1};

        std::unique_ptr<std::thread> thread;
        bool createdTextData{false}, allowLineWrap{true}, shrinkOnOverflow{true};

        int prevFontSize{0};
        glm::vec4 prevColor = glm::vec4(0);
        int maxWidth{0};

        bool continuousThread{false}, runContinuousThread{true}, continuousThreadReady{false};
        std::condition_variable threadBlockingCV;
        std::mutex threadBlockingMutex;
    };
}