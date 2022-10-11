#include "file_explorer/file_explorer.hpp"

void FileExplorer::createSidebarGUI()
{
    sidebar = std::make_unique<lc_gui::Element>("sidebar");
    container->addChild(sidebar.get());

    float btnHeight = renderer->getStyle("directory-btn")->sizeProperties.at("height").value;
    std::string defaultFoldersTextOutput;

    size_t defaultFoldersSize = defaultFolders.size();
    defaultFoldersDirectories.resize(defaultFoldersSize);

    size_t i = 0;
    for (auto it = defaultFolders.begin(); it != defaultFolders.end(); it++, i++)
    {
        if (it->second.back() != '/')
            it->second.push_back('/');

        auto &d = defaultFoldersDirectories[i];

        d.name = it->first;
        d.isFolder = true;

        d.btn = std::make_unique<lc_gui::ButtonElement>("directory-btn", "directory-btn-click", defaultFolderBtnCallback);
        d.btn->id.push_back(i);
        sidebar->addChild(d.btn.get());

        auto &y = d.btn->elementStyle.sizeProperties["top"];
        y.isSet = true;
        y.unit = lc_gui::Style::PIXELS;
        y.value = i * btnHeight;

        d.icon = std::make_unique<lc_gui::ImageElement>(folderIconTexture, false, "directory-icon");
        d.btn->addChild(d.icon.get());

        if (!defaultFoldersTextOutput.empty())
            defaultFoldersTextOutput.push_back('\n');

        defaultFoldersTextOutput += d.name;
    }

    defaultFoldersText = std::make_unique<lc_gui::TextElement>(font, defaultFoldersTextOutput, "directory-text", false, false, false);
    sidebar->addChild(defaultFoldersText.get());
}