#pragma once

#include "common.hpp"

namespace lc_gui
{
    std::string textFileData(const std::string &filePath);

    class TextureData
    {
    public:
        TextureData(){}
        TextureData(const std::string &filePath);

        void load(const std::string &filePath);

        std::vector<unsigned char> bytes;
        int width{0}, height{0};
        GLenum format{GL_NONE};
    };
}