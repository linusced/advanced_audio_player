#pragma once

#include "shader.hpp"

namespace lc_gui
{
    class Texture : public ID
    {
    public:
        Texture() {}
        Texture(const std::vector<unsigned char> &bytes, int width, int height, GLenum format, int unit = 0, GLenum dataType = GL_UNSIGNED_BYTE, GLenum wrapX = GL_REPEAT, GLenum wrapY = GL_REPEAT, GLenum scaleAlgorithm = GL_NEAREST, const glm::vec4 &borderColor = glm::vec4(-1.0f));
        Texture(int width, int height, GLenum format, int unit = 0, GLenum dataType = GL_UNSIGNED_BYTE, GLenum wrapX = GL_REPEAT, GLenum wrapY = GL_REPEAT, GLenum scaleAlgorithm = GL_NEAREST, const glm::vec4 &borderColor = glm::vec4(-1.0f));
        ~Texture();

        void update(const std::vector<unsigned char> &bytes, int width, int height, GLenum format, int unit = 0, GLenum dataType = GL_UNSIGNED_BYTE, GLenum wrapX = GL_REPEAT, GLenum wrapY = GL_REPEAT, GLenum scaleAlgorithm = GL_NEAREST, const glm::vec4 &borderColor = glm::vec4(-1.0f));

        void activate(Shader *shader, const std::string &uniformName);
        void bind();
        void unbind();

        const unsigned int &getUnit() const;

        const int &getWidth() const;
        const int &getHeight() const;
        GLenum getFormat() const;

    private:
        unsigned int unit{0};

        int width{0}, height{0};
        GLenum format{GL_NONE};
    };
}