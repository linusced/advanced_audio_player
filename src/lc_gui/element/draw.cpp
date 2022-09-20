#include "lc_gui/element.hpp"

void lc_gui::Element::draw(Shader *shader)
{
    bool overflowHidden{false}, drawScrollMesh{false};
    if (prevStyle.miscProperties.at("overflow").isSet && prevStyle.miscProperties.at("overflow").value != "visible")
    {
        drawScrollMesh = true;
        if (!glIsEnabled(GL_DEPTH_TEST))
        {
            glClear(GL_DEPTH_BUFFER_BIT);
            glDepthFunc(GL_ALWAYS);
            glEnable(GL_DEPTH_TEST);
            overflowHidden = true;
        }
    }

    containerMesh.draw(shader);

    if (borderMesh.indices.size() > 0)
    {
        if (overflowHidden)
            glDisable(GL_DEPTH_TEST);

        borderMesh.draw(shader);

        if (overflowHidden)
            glEnable(GL_DEPTH_TEST);
    }

    glDepthFunc(GL_EQUAL);

    for (auto &m : extraMeshes)
        m->draw(shader);

    if (mesh.indices.size() > 0)
        mesh.draw(shader);

    for (auto &e : children)
        if (e->isVisible)
            e->draw(shader);

    if (drawScrollMesh && scrollMesh.indices.size() > 0)
        scrollMesh.draw(shader);

    if (overflowHidden)
    {
        glClear(GL_DEPTH_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
    }
}