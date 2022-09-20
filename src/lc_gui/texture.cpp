#include "lc_gui/texture.hpp"

lc_gui::Texture::Texture(const std::vector<unsigned char> &bytes, int width, int height, GLenum format, int unit, GLenum dataType, GLenum wrapX, GLenum wrapY, GLenum scaleAlgorithm, const glm::vec4 &borderColor)
{
    update(bytes, width, height, format, unit, dataType, wrapX, wrapY, scaleAlgorithm, borderColor);
}
lc_gui::Texture::Texture(int width, int height, GLenum format, int unit, GLenum dataType, GLenum wrapX, GLenum wrapY, GLenum scaleAlgorithm, const glm::vec4 &borderColor)
{
    update({}, width, height, format, unit, dataType, wrapX, wrapY, scaleAlgorithm, borderColor);
}
lc_gui::Texture::~Texture()
{
    glDeleteTextures(1, &id);
}

void lc_gui::Texture::update(const std::vector<unsigned char> &bytes, int width, int height, GLenum format, int unit, GLenum dataType, GLenum wrapX, GLenum wrapY, GLenum scaleAlgorithm, const glm::vec4 &borderColor)
{
    this->width = width;
    this->height = height;
    this->format = format;
    this->unit = unit;

    if (id == (unsigned int)-1)
        glGenTextures(1, &id);

    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapX);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapY);

    if ((wrapX == GL_CLAMP_TO_BORDER || wrapY == GL_CLAMP_TO_BORDER) && borderColor.x != -1)
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(borderColor));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, scaleAlgorithm);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, scaleAlgorithm);

    glTexImage2D(GL_TEXTURE_2D, 0, format == GL_DEPTH_COMPONENT ? GL_DEPTH_COMPONENT : GL_RGBA, width, height, 0, format, dataType, bytes.empty() ? nullptr : bytes.data());
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void lc_gui::Texture::activate(Shader *shader, const std::string &uniformName)
{
    glUniform1i(glGetUniformLocation(shader->getID(), uniformName.c_str()), unit);
}
void lc_gui::Texture::bind()
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, id);
}
void lc_gui::Texture::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

const unsigned int &lc_gui::Texture::getUnit() const
{
    return unit;
}

const int &lc_gui::Texture::getWidth() const
{
    return width;
}
const int &lc_gui::Texture::getHeight() const
{
    return height;
}
GLenum lc_gui::Texture::getFormat() const
{
    return format;
}