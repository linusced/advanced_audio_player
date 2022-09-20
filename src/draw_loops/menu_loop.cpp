#include "draw_loops/menu_loop.hpp"

MenuLoop::MenuLoop(std::string assetsFilePath, std::string appSupportFilePath, lc_gui::TextRenderer::Font *font, FileExplorer *fileExplorer, lc_gui::Window *window)
    : lc_gui::DrawLoop(window, ""), assetsFilePath(assetsFilePath), appSupportFilePath(appSupportFilePath), font(font), fileExplorer(fileExplorer)
{
}

void MenuLoop::loop()
{
    bool resizeInit{true}, keyDown{false};

    while (!window->shouldClose())
    {
        window->update();

        if (window->hasResized() || resizeInit)
        {
            resizeInit = false;
            fileExplorer->resize();
        }

        if (glfwGetKey(*window, GLFW_KEY_SPACE))
        {
            if (!keyDown)
            {
                fileExplorer->open(fileExplorer->getDefaultFolders().at("Documents"));
                keyDown = true;
            }
        }
        else
            keyDown = false;

        if (fileExplorer->isActive())
        {
            fileExplorer->update();

            if (fileExplorer->userHasSelectedAFile())
                std::cout << fileExplorer->getSelectedFilePath() << '\n';
        }

        glClear(GL_COLOR_BUFFER_BIT);

        if (fileExplorer->isActive())
            fileExplorer->draw();

        window->swapBuffers();
    }
}