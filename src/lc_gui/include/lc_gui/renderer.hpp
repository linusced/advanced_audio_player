#pragma once

#include "element.hpp"

namespace lc_gui
{
    class Renderer
    {
    public:
        Renderer(std::string cssCode, Window *window);

        void resize();
        void update();
        void draw();

        void addElement(Element *e);
        void addChild(Element *e);

        const bool &frameInputsActive() const;

        const Style *getStyle(const std::string &name) const;

    private:
        static const std::string vertexShaderCode, fragmentShaderCode;

        void parseCSS(std::string &css);
        void getStyleSize(Style::SIZE &output, std::string &str);
        void getStyleColor(Style::COLOR &output, std::string &str);

        void resizeRecursive(Element *e, const Layout &parentLayout);
        void updateStyleRecursive(Element *e, const Layout &parentLayout, bool styleChangeChildren);

        void updateRecursive(Element *e);
        bool inputsRecursive(Element *e, const Layout &parentLayout, glm::ivec2 relativeCursorPosition);

        bool hasStyleChangedRecursive(Element *e);

        Window *window;
        std::unique_ptr<Shader> shader;
        glm::ivec2 windowSize = glm::ivec2(0);
        Layout windowLayout;

        std::vector<Style> styles;
        std::vector<Element *> elements;

        bool elementRequireInputs{false}, inputsActive{false};
    };
}