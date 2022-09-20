#include "lc_gui/lc_gui.hpp"

void lc_gui::init(bool multisample)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    stbi_set_flip_vertically_on_load(true);

    if (multisample && Window::getContentScale() == 1)
        glfwWindowHint(GLFW_SAMPLES, 8);
}

void lc_gui::terminate()
{
    glfwTerminate();
}

void lc_gui::enableBlend()
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
}