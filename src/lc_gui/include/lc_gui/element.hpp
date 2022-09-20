#pragma once

#include "style.hpp"
#include "window.hpp"
#include "mesh.hpp"

namespace lc_gui
{
    class ClassList
    {
    public:
        std::vector<std::string>::iterator begin();
        std::vector<std::string>::iterator end();
        const size_t &size() const;
        std::string &operator[](size_t index);
        bool contains(const std::string &str);

        void remove(const std::string &str);
        void add(const std::string &str);
        void clear();

        void operator=(const std::string &className);

    private:
        std::vector<std::string> strArray;
    };

    class Element
    {
    public:
        Element(const std::string &className);
        virtual ~Element() {}

        void resize(Style &style, Layout &layout, glm::ivec2 &windowSize);
        void draw(Shader *shader);

        virtual void resizeMesh(const Style &style, const Layout &layout, const glm::ivec2 &windowSize);
        virtual void update();
        virtual bool inputs(const glm::ivec2 &relativeCursorPosition);

        void addChild(Element *child);

        const Style &getPreviousStyle() const;
        const Layout &getPreviousLayout() const;
        const Layout &getPreviousContainerLayout() const;
        int getMaxY() const;

        ClassList classList;
        Window *window{nullptr};
        std::vector<Element *> children;
        Element *parentElement{nullptr};

        bool styleChange{false}, requireInputs{false};
        int scrollY{0};
        bool isVisible{true};

        Style elementStyle;
        std::string id;

    protected:
        Style prevStyle;
        Layout prevLayout;

        Mesh2D mesh;
        std::vector<std::unique_ptr<Mesh2D>> extraMeshes;

        int bottomY{0};
        bool lmbDown{false};
        glm::ivec2 lmbDownStart = glm::ivec2(-1);

        Mesh2D containerMesh, borderMesh, scrollMesh;
        int maxScrollY{0}, scrollMeshWidth{0}, scrollMeshOffsetY{0}, scrollMeshHeight{0},
            lmbDownScrollMeshOffsetY{0};
        Layout prevContainerLayout;

        void containerMeshDefault(const glm::vec4 &bgColor, const Layout &layout);
        void containerMeshBorderWidth(const int &borderWidth, const glm::vec4 &bgColor, const glm::vec4 &borderColor, const Layout &layout);
        void containerMeshBorderRadius(const int &borderRadius, const int &borderWidth, const glm::vec4 &bgColor, const glm::vec4 &borderColor, const Layout &layout);
    };
}