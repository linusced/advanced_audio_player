#include "lc_gui/mesh.hpp"

lc_gui::Mesh2D::Mesh2D(const std::vector<Vertex2D> &vertices, const std::vector<unsigned int> &indices, Texture *texture)
    : vertices(vertices), indices(indices), texture(texture)
{
    applyVertexChange();
}

void lc_gui::Mesh2D::applyVertexChange()
{
    std::vector<Vertex2D> vertOut = vertices;
    for (Vertex2D &v : vertOut)
    {
        v.position = {v.position.x * 2 - 1, v.position.y * -2 + 1};
        v.textureCoords.y *= -1;
    }

    if (vao)
    {
        vao->bind();
        vbo = std::make_unique<VBO>(vertOut.size(), sizeof(Vertex2D), vertOut.data());
        ebo = std::make_unique<EBO>(indices);
    }
    else
    {
        vao = std::make_unique<VAO>();
        vao->bind();
        vbo = std::make_unique<VBO>(vertOut.size(), sizeof(Vertex2D), vertOut.data());
        ebo = std::make_unique<EBO>(indices);
    }
    vbo->bind();

    vao->linkAttrib(0, 2, GL_FLOAT, sizeof(Vertex2D), (void *)0);
    vao->linkAttrib(1, 2, GL_FLOAT, sizeof(Vertex2D), (void *)(2 * sizeof(float)));
    vao->linkAttrib(2, 4, GL_FLOAT, sizeof(Vertex2D), (void *)(4 * sizeof(float)));

    vao->unbind();
    vbo->unbind();
    ebo->unbind();
}

void lc_gui::Mesh2D::draw(Shader *shader)
{
    vao->bind();

    glUniform1i(glGetUniformLocation(shader->getID(), "useTexture"), texture != nullptr);

    if (texture != nullptr)
    {
        texture->activate(shader, "texture0");
        texture->bind();
    }

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    vao->unbind();
}