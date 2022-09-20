#pragma once

#include "../element.hpp"

namespace lc_gui
{
    class ImageElement : public Element
    {
    public:
        ImageElement(Texture *imageTexture, bool scaleToCover, const std::string &className);

        void resizeMesh(const Style &style, const Layout &layout, const glm::ivec2 &windowSize) override;

        void setImage(Texture *image);
        void setTextureCoords(glm::vec2 x, glm::vec2 y);

    private:
        glm::vec2 textureCoordsX = glm::vec2(0.0f, 1.0f), textureCoordsY = glm::vec2(0.0f, 1.0f);
        bool scaleToCover;
    };
}