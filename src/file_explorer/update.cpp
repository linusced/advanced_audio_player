#include "file_explorer/file_explorer.hpp"

void FileExplorer::update()
{
    if (dirPathUpdate)
        updateDirectories();

    if (cancel)
    {
        cancel = fileSelected = active = false;
        directoriesContainer->children.clear();
        directoriesContainer->styleChange = true;
        directories.clear();
        dirPath.clear();
        return;
    }

    renderer->update();

    if (renderer->frameInputsActive())
        return;

    if (glfwGetKey(*window, GLFW_KEY_ESCAPE))
    {
        if (!keyDown[GLFW_KEY_ESCAPE])
        {
            cancel = true;
            keyDown[GLFW_KEY_ESCAPE] = true;
        }
    }
    else
        keyDown[GLFW_KEY_ESCAPE] = false;

    if (glfwGetKey(*window, GLFW_KEY_PAGE_UP))
    {
        if (!keyDown[GLFW_KEY_PAGE_UP])
        {
            parentFolderBtn->click();
            keyDown[GLFW_KEY_PAGE_UP] = true;
        }
    }
    else
        keyDown[GLFW_KEY_PAGE_UP] = false;

    if (glfwGetKey(*window, GLFW_KEY_ENTER) || glfwGetKey(*window, GLFW_KEY_KP_ENTER) || glfwGetKey(*window, GLFW_KEY_PAGE_DOWN))
    {
        if (!keyDown[GLFW_KEY_ENTER])
        {
            goBtn->click();
            keyDown[GLFW_KEY_ENTER] = true;
        }
    }
    else
        keyDown[GLFW_KEY_ENTER] = false;

    if (glfwGetKey(*window, GLFW_KEY_DOWN) || glfwGetKey(*window, GLFW_KEY_RIGHT))
    {
        if (!keyDown[GLFW_KEY_DOWN])
        {
            if (!directories.empty())
                prevClickIndex = (prevClickIndex + 1) % directories.size();

            if (prevClickIndex != -1)
            {
                prevClickTime = 0.0;
                directories[prevClickIndex].btn->click();
            }

            keyDown[GLFW_KEY_DOWN] = true;
        }
    }
    else
        keyDown[GLFW_KEY_DOWN] = false;

    if (glfwGetKey(*window, GLFW_KEY_UP) || glfwGetKey(*window, GLFW_KEY_LEFT))
    {
        if (!keyDown[GLFW_KEY_UP])
        {
            prevClickIndex--;
            if (prevClickIndex < 0)
                prevClickIndex = directories.size() - 1;

            if (prevClickIndex != -1)
            {
                prevClickTime = 0.0;
                directories[prevClickIndex].btn->click();
            }

            keyDown[GLFW_KEY_UP] = true;
        }
    }
    else
        keyDown[GLFW_KEY_UP] = false;
}