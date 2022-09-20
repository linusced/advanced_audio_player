#include "lc_gui/basic_elements/image_element.hpp"

lc_gui::ImageElement::ImageElement(Texture *imageTexture, bool scaleToCover, const std::string &className)
    : Element(className), scaleToCover(scaleToCover)
{
    mesh.vertices.resize(4);
    mesh.indices = {0, 1, 2, 0, 2, 3};
    mesh.texture = imageTexture;
}

void lc_gui::ImageElement::resizeMesh(const Style &style, const Layout &layout, const glm::ivec2 &windowSize)
{
    float textureAspectRatio = (mesh.texture->getWidth() * (textureCoordsX[1] - textureCoordsX[0])) / (float)(mesh.texture->getHeight() * (textureCoordsY[1] - textureCoordsY[0])),
          layoutAspectRatio = layout.width / (float)layout.height,
          relativeAspectRatio = textureAspectRatio / layoutAspectRatio;

    glm::vec2 outCoordsX = textureCoordsX, outCoordsY = textureCoordsY;

    if (scaleToCover)
    {
        if (relativeAspectRatio < 1.0f)
        {
            outCoordsY[0] = (1.0f - relativeAspectRatio) / 2.0f;
            outCoordsY[1] = 1.0f - (1.0f - relativeAspectRatio) / 2.0f;

            outCoordsY[0] = outCoordsY[0] * (textureCoordsY[1] - textureCoordsY[0]) + textureCoordsY[0];
            outCoordsY[1] = outCoordsY[1] * (textureCoordsY[1] - textureCoordsY[0]) + textureCoordsY[0];
        }
        else if (relativeAspectRatio > 1.0f)
        {
            relativeAspectRatio = 1.0f / relativeAspectRatio;
            outCoordsX[0] = ((1.0f - relativeAspectRatio) / 2.0f);
            outCoordsX[1] = 1.0f - (1.0f - relativeAspectRatio) / 2.0f;

            outCoordsX[0] = outCoordsX[0] * (textureCoordsX[1] - textureCoordsX[0]) + textureCoordsX[0];
            outCoordsX[1] = outCoordsX[1] * (textureCoordsX[1] - textureCoordsX[0]) + textureCoordsX[0];
        }
    }

    mesh.vertices[0].textureCoords = {outCoordsX[0], outCoordsY[0]};
    mesh.vertices[1].textureCoords = {outCoordsX[1], outCoordsY[0]};
    mesh.vertices[2].textureCoords = {outCoordsX[1], outCoordsY[1]};
    mesh.vertices[3].textureCoords = {outCoordsX[0], outCoordsY[1]};

    Layout outLayout = layout;
    if (!scaleToCover)
    {
        relativeAspectRatio = 1.0f / relativeAspectRatio;
        if (relativeAspectRatio < 1.0f)
        {
            outLayout.height = (1.0f - (1.0f - relativeAspectRatio) / 2.0f) * layout.height;
            outLayout.y += (layout.height - outLayout.height) / 2;
        }
        else if (relativeAspectRatio > 1.0f)
        {
            relativeAspectRatio = 1.0f / relativeAspectRatio;
            outLayout.width = (1.0f - (1.0f - relativeAspectRatio) / 2.0f) * layout.width;
            outLayout.x += (layout.width - outLayout.width) / 2;
        }
    }

    mesh.vertices[0].position = {outLayout.x, outLayout.y};
    mesh.vertices[1].position = {outLayout.x + outLayout.width, outLayout.y};
    mesh.vertices[2].position = {outLayout.x + outLayout.width, outLayout.y + outLayout.height};
    mesh.vertices[3].position = {outLayout.x, outLayout.y + outLayout.height};

    for (auto &v : mesh.vertices)
        v.position /= windowSize;

    mesh.applyVertexChange();
}

void lc_gui::ImageElement::setImage(Texture *image)
{
    mesh.texture = image;
}

void lc_gui::ImageElement::setTextureCoords(glm::vec2 x, glm::vec2 y)
{
    textureCoordsX = x;
    textureCoordsY = y;
    styleChange = true;
}