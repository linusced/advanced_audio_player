#include "draw_loops/main_loop.hpp"

void MainLoop::updateFileManager(bool reset)
{
    updateFileManagerNextFrame = false;
    fileManagerContainer->styleChange = true;

    size_t numFiles = audioPlayer->getNumFiles();
    if (reset)
    {
        fileManagerContainer->children.clear();

        fileManagerResetBtn->isVisible = fileManagerSaveBtn->isVisible = false;

        fileManagerContainer->addChild(fileManagerResetBtn.get());
        fileManagerContainer->addChild(fileManagerSaveBtn.get());
        fileManagerContainer->addChild(fileManagerAddBtn.get());

        fileManagerItemsArr.clear();
        fileManagerItemsArr.resize(numFiles);
    }
    else
    {
        numFiles = fileManagerItemsArr.size();
        fileManagerResetBtn->isVisible = fileManagerSaveBtn->isVisible = true;
    }

    const float itemHeight = guiRenderer->getStyle("file-manager-item")->sizeProperties.at("height").value;

    auto &height = fileManagerContainer->elementStyle.sizeProperties["height"];
    height.isSet = true;
    height.unit = lc_gui::Style::PIXELS;
    height.value = numFiles * itemHeight + guiRenderer->getStyle("file-manager-reset")->sizeProperties.at("height").value;

    for (size_t i = 0; i < numFiles; i++)
    {
        auto &item = fileManagerItemsArr[i];

        if (reset)
            item.waveformIndex = item.initialWaveformIndex = i;

        if (!item.container)
        {
            item.container = std::make_unique<lc_gui::Element>("file-manager-item");
            fileManagerContainer->addChild(item.container.get());
        }

        auto &top = item.container->elementStyle.sizeProperties["top"];
        top.isSet = true;
        top.unit = lc_gui::Style::PIXELS;
        top.value = i * itemHeight;

        if (!item.name)
        {
            std::string indexStr = std::to_string(i),
                        nameStr;

            if (item.filePath.empty())
            {
                nameStr = audioPlayer->getFileName(i);
            }
            else
            {
                size_t lastSlash = item.filePath.find_last_of('/') + 1;
                nameStr = item.filePath.substr(lastSlash, item.filePath.find_last_of('.') - lastSlash);
            }

            nameStr[0] = toupper(nameStr[0]);
            for (size_t i = 1; i < nameStr.size() - 1; i++)
                if (nameStr[i] == ' ' || nameStr[i] == '_' || nameStr[i] == '-')
                {
                    nameStr[i] = ' ';
                    nameStr[i + 1] = toupper(nameStr[i + 1]);
                    i++;
                }

            item.name = std::make_unique<lc_gui::TextElement>(font, nameStr, "file-manager-name", false, false);
            item.container->addChild(item.name.get());

            item.moveDownBtn = std::make_unique<lc_gui::ButtonElement>(fileManagerBtnCallback, "file-manager-move-down", "btn-click");
            item.moveDownBtn->id = "move-down-" + indexStr;
            item.container->addChild(item.moveDownBtn.get());

            item.moveDownBtnIcon = std::make_unique<lc_gui::ImageElement>(moveDownIcon.get(), false, "file-manager-btn-icon");
            item.moveDownBtn->addChild(item.moveDownBtnIcon.get());

            item.moveUpBtn = std::make_unique<lc_gui::ButtonElement>(fileManagerBtnCallback, "file-manager-move-up", "btn-click");
            item.moveUpBtn->id = "move-up-" + indexStr;
            item.container->addChild(item.moveUpBtn.get());

            item.moveUpBtnIcon = std::make_unique<lc_gui::ImageElement>(moveUpIcon.get(), false, "file-manager-btn-icon");
            item.moveUpBtn->addChild(item.moveUpBtnIcon.get());

            item.removeBtn = std::make_unique<lc_gui::ButtonElement>(fileManagerBtnCallback, "file-manager-remove", "btn-click");
            item.removeBtn->id = "remove-" + indexStr;
            item.container->addChild(item.removeBtn.get());

            item.removeBtnIcon = std::make_unique<lc_gui::ImageElement>(removeIcon.get(), false, "file-manager-btn-icon");
            item.removeBtn->addChild(item.removeBtnIcon.get());
        }
    }
}