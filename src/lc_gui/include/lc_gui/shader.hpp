#pragma once

#include "common.hpp"

namespace lc_gui
{
    class Shader : public ID
    {
    public:
        Shader(const std::string &vertexCode, const std::string &fragmentCode);
        ~Shader();

        void activate();

    private:
        static void compileErrors(unsigned int shader, const std::string &type);
    };
}