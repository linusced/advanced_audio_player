#pragma once

#include "shader.hpp"
#include "texture.hpp"

namespace lc_gui
{
    struct Vertex2D
    {
        glm::vec2 position = glm::vec2(0.0f), textureCoords = glm::vec2(0.0f);
        glm::vec4 color = glm::vec4(0.0f);
    };

    class VAO : public ID
    {
    public:
        VAO();
        ~VAO();

        void linkAttrib(unsigned intlayout, unsigned intnumComponents, GLenum type, GLsizeiptr stride, void *offset);
        void bind();
        void unbind();
    };

    class VBO : public ID
    {
    public:
        VBO(size_t verticesSize, size_t vertexSizeof, const void *verticesData);
        ~VBO();

        void bind();
        void unbind();
    };

    class EBO : public ID
    {
    public:
        EBO(const std::vector<unsigned int> &indices);
        ~EBO();

        void bind();
        void unbind();
    };

    class Mesh2D
    {
    public:
        Mesh2D() {}
        Mesh2D(const std::vector<Vertex2D> &vertices, const std::vector<unsigned int> &indices, Texture *texture = nullptr);
        virtual ~Mesh2D() {}

        virtual void applyVertexChange();

        virtual void draw(Shader *shader);

        std::vector<Vertex2D> vertices;
        std::vector<unsigned int> indices;
        Texture *texture{nullptr};

    protected:
        std::unique_ptr<VAO> vao;
        std::unique_ptr<VBO> vbo;
        std::unique_ptr<EBO> ebo;
    };
}