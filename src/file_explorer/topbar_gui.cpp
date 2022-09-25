#include "file_explorer/file_explorer.hpp"

void FileExplorer::createTopbarGUI()
{
    topbar = std::make_unique<lc_gui::Element>("topbar");
    container->addChild(topbar.get());

    currentPath = std::make_unique<lc_gui::TextElement>(font, "", "current-path", true, true);
    topbar->addChild(currentPath.get());

    parentFolderBtn = std::make_unique<lc_gui::ButtonElement>(topbarBtnCallback, "parent-folder-btn", "parent-folder-btn-click");
    parentFolderBtn->id = "parent-folder";
    topbar->addChild(parentFolderBtn.get());

    parentFolderBtnText = std::make_unique<lc_gui::TextElement>(font, "Parent folder", "btn-text", false, false);
    parentFolderBtn->addChild(parentFolderBtnText.get());

    goBtn = std::make_unique<lc_gui::ButtonElement>(topbarBtnCallback, "go-btn", "go-btn-click");
    goBtn->id = "go";
    topbar->addChild(goBtn.get());

    goBtnText = std::make_unique<lc_gui::TextElement>(font, "Go", "btn-text", false, false);
    goBtn->addChild(goBtnText.get());

    cancelBtn = std::make_unique<lc_gui::ButtonElement>(topbarBtnCallback, "cancel-btn", "cancel-btn-click");
    cancelBtn->id = "cancel";
    topbar->addChild(cancelBtn.get());

    cancelBtnText = std::make_unique<lc_gui::TextElement>(font, "Cancel", "btn-text", false, false);
    cancelBtn->addChild(cancelBtnText.get());
}