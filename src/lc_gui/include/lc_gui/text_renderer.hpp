#pragma once

#include "texture.hpp"
#include "style.hpp"

namespace lc_gui
{
    class TextRenderer
    {
    public:
        struct Font
        {
        public:
            Font(const std::string &filePath);

            const uint8_t *data() const;
            size_t size() const;

        private:
            std::vector<uint8_t> buffer;
        };
        struct CharacterColor
        {
            glm::vec4 color = glm::vec4(1.0f);
            std::vector<int> characterIndices;
        };

        std::vector<uint8_t> textureBytes;
        int textureWidth{0}, textureHeight{0};
        std::vector<Layout> characterLayouts, lineLayouts;

        void render(const std::string &text, const glm::vec4 &color, int fontSize, int maxWidth, const Font *font);
        void render(const std::string &text, const glm::vec4 &color, int fontSize, int maxWidth, const Font *font, const std::vector<CharacterColor> &characterColors);
    };
}