#include "file_explorer/file_explorer.hpp"

void FileExplorer::createBtmbarGUI()
{
    btmbar = std::make_unique<lc_gui::Element>("btmbar");
    container->addChild(btmbar.get());

    btmbarContent = std::make_unique<lc_gui::Element>("btmbar-content");
    btmbar->addChild(btmbarContent.get());

    fileNameInputLabel = std::make_unique<lc_gui::TextElement>(font, "File name:", "file-name-input-label", false, false);
    btmbarContent->addChild(fileNameInputLabel.get());

    fileNameInput = std::make_unique<lc_gui::TextInputElement>(font, "", "file-name-input");
    btmbarContent->addChild(fileNameInput.get());

    btmbarSelectBtn = std::make_unique<lc_gui::ButtonElement>("btmbar-select-btn", "go-btn-click", btmbarSelectCallback);
    btmbarContent->addChild(btmbarSelectBtn.get());

    btmbarSelectText = std::make_unique<lc_gui::TextElement>(font, "Select", "btn-text", false, false);
    btmbarSelectBtn->addChild(btmbarSelectText.get());
}