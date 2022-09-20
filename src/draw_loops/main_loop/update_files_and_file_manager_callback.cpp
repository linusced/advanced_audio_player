#include "draw_loops/main_loop.hpp"

void MainLoop::updateFiles()
{
    int removeFiles = 0;
    for (auto &item : fileManagerItemsArr)
        if (item.waveformIndex == -1)
        {
            removeFiles++;
        }
        else
        {
            if (item.waveformIndex != item.initialWaveformIndex)
            {
                audioPlayer->swapFileLocations(item.initialWaveformIndex, item.waveformIndex);

                for (auto &item2 : fileManagerItemsArr)
                    if (item2.initialWaveformIndex == item.waveformIndex)
                    {
                        item2.initialWaveformIndex = item.initialWaveformIndex;
                        break;
                    }
            }
        }

    if (removeFiles > 0)
        audioPlayer->removeFiles(audioPlayer->getNumFiles() - removeFiles, removeFiles);

    audioPlayer->setFilePlayOffset(0UL, 0);

    const size_t numFiles = audioPlayer->getNumFiles();
    for (size_t i = 1; i < numFiles; i++)
        audioPlayer->setFilePlayOffset(audioPlayer->getFilePlayOffset(i - 1) + audioPlayer->getFileDuration(i - 1), i);

    updateFileManagerNextFrame = updateWaveformDataNextFrame = true;
}

void MainLoop::createFileManagerCallback()
{
    fileManagerBtnCallback = [this](lc_gui::ButtonElement *btn)
    {
        size_t lastDash = btn->id.find_last_of('-');
        std::string type = btn->id.substr(0, lastDash);
        int index = std::stoi(btn->id.substr(lastDash + 1));

        if (type == "reset")
        {
            updateFileManagerNextFrame = true;
        }
        else if (type == "save")
        {
            const size_t numFiles = audioPlayer->getNumFiles();
            std::vector<std::string> audioFilePaths;

            for (auto &item : fileManagerItemsArr)
                if (item.initialWaveformIndex == -1)
                {
                    item.initialWaveformIndex = numFiles + audioFilePaths.size();
                    audioFilePaths.push_back(item.filePath);
                }

            if (audioFilePaths.empty())
            {
                updateFiles();
            }
            else
            {
                audioPlayer->loadFiles(audioFilePaths);
                resetLoading = updateFilesAfterLoading = true;
            }

            toggleFileManagerBtn->click();
        }
        else if (type == "add")
        {
            openFileExplorer = true;
            explorerType = FILE_EXPLORER_ADD_FILE;
        }
        else
        {

            const float itemHeight = guiRenderer->getStyle("file-manager-item")->sizeProperties.at("height").value;
            size_t numFiles = 0;
            for (auto &item : fileManagerItemsArr)
                if (item.waveformIndex != -1)
                    numFiles++;

            if (type == "move-down")
            {
                if (fileManagerItemsArr[index].waveformIndex == numFiles - 1)
                    return;

                for (auto &item : fileManagerItemsArr)
                    if (item.waveformIndex == fileManagerItemsArr[index].waveformIndex + 1)
                    {
                        item.waveformIndex--;
                        item.container->elementStyle.sizeProperties["top"].value -= itemHeight;
                        break;
                    }

                fileManagerItemsArr[index].waveformIndex++;
                fileManagerItemsArr[index].container->elementStyle.sizeProperties["top"].value += itemHeight;
            }
            else if (type == "move-up")
            {
                if (fileManagerItemsArr[index].waveformIndex == 0)
                    return;

                for (auto &item : fileManagerItemsArr)
                    if (item.waveformIndex == fileManagerItemsArr[index].waveformIndex - 1)
                    {
                        item.waveformIndex++;
                        item.container->elementStyle.sizeProperties["top"].value += itemHeight;
                        break;
                    }

                fileManagerItemsArr[index].waveformIndex--;
                fileManagerItemsArr[index].container->elementStyle.sizeProperties["top"].value -= itemHeight;
            }
            else
            {
                int removeIndex = fileManagerItemsArr[index].waveformIndex;
                fileManagerItemsArr[index].waveformIndex = -1;
                fileManagerItemsArr[index].container->isVisible = false;

                numFiles--;
                fileManagerContainer->elementStyle.sizeProperties["height"].value = numFiles * itemHeight + guiRenderer->getStyle("file-manager-reset")->sizeProperties.at("height").value;

                for (auto &item : fileManagerItemsArr)
                    if (item.waveformIndex != -1)
                    {
                        if (item.waveformIndex > removeIndex)
                            item.waveformIndex--;

                        item.container->elementStyle.sizeProperties["top"].value = item.waveformIndex * itemHeight;
                    }
            }

            fileManagerContainer->styleChange = true;
            fileManagerResetBtn->isVisible = true;
            fileManagerSaveBtn->isVisible = false;

            for (auto &item : fileManagerItemsArr)
                if (item.waveformIndex != -1)
                {
                    fileManagerSaveBtn->isVisible = true;
                    break;
                }
        }
    };
}