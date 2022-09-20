#include "file_explorer/file_explorer.hpp"

void FileExplorer::createCallbacks()
{
    directoryBtnCallback = [this](lc_gui::ButtonElement *btn)
    {
        for (auto &d : directories)
        {
            d.btn->classList.remove("directory-btn-active");
            d.btn->styleChange = true;
        }

        int index = btn->id.front();
        const double &currentTime = window->getCurrentTime();

        if (index == prevClickIndex && currentTime - prevClickTime < DOUBLE_CLICK_MAX_INTERVAL)
        {
            if (directories[index].isFolder)
            {
                dirPath += directories[index].name;
                dirPathUpdate = true;
            }
            else
            {
                selectedFile = dirPath + directories[index].name;
                fileSelected = true;
            }
        }
        else
        {
            btn->classList.add("directory-btn-active");
            prevClickIndex = index;
            prevClickTime = currentTime;
            goBtn->isVisible = true;

            updateCurrentPath(dirPath + directories[index].name);
        }
    };

    defaultFolderBtnCallback = [this](lc_gui::ButtonElement *btn)
    {
        dirPath = defaultFolders[defaultFoldersDirectories[btn->id.front()].name];
        dirPathUpdate = true;
    };

    topbarBtnCallback = [this](lc_gui::ButtonElement *btn)
    {
        if (btn->id == "parent-folder")
        {
            dirPath = dirPath.substr(0, dirPath.size() - 1);
            dirPath = dirPath.substr(0, dirPath.find_last_of('/'));
            dirPathUpdate = true;
        }
        else if (btn->id == "go")
        {
            if (prevClickIndex != -1)
            {
                prevClickTime = window->getCurrentTime();
                directories[prevClickIndex].btn->click();
            }
        }
        else if (btn->id == "cancel")
            cancel = true;
    };

    btmbarSelectCallback = [this](lc_gui::ButtonElement *btn)
    {
        if (!fileNameInput->getText().empty())
        {
            selectedFile = dirPath + fileNameInput->getText();
            fileSelected = true;
        }
    };
}