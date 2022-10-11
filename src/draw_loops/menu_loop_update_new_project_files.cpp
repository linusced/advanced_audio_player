#include "draw_loops/menu_loop.hpp"

void MenuLoop::updateNewProjectFiles(const std::vector<std::string> &files)
{
    newProjectFilesContainer->children.clear();
    newProjectFilesArr.clear();
    newProjectFilesContainer->styleChange = true;

    int index{0},
        yOffset = guiRenderer->getStyle("project-container")->sizeProperties.at("height").value + guiRenderer->getStyle("project-container")->sizeProperties.at("border-width").value * 2;

    for (auto &path : files)
    {
        newProjectFilesArr.push_back({});
        auto &f = newProjectFilesArr.back();
        f.absPath = path;

        f.container = std::make_unique<lc_gui::Element>("project-container");
        auto &y = f.container->elementStyle.sizeProperties["top"];
        y.isSet = true;
        y.unit = lc_gui::Style::PIXELS;
        y.value = index * yOffset;
        newProjectFilesContainer->addChild(f.container.get());

        size_t fileNameIndex = path.find_last_of('/') + 1;
        std::string formattedName = path.substr(fileNameIndex, path.find_last_of('.') - fileNameIndex);
        formattedName[0] = std::toupper(formattedName[0]);

        for (size_t i = 1; i < formattedName.size() - 1; i++)
            if (formattedName[i] == '-' || formattedName[i] == '_')
            {
                formattedName[i] = ' ';
                formattedName[i + 1] = std::toupper(formattedName[i + 1]);
            }

        f.text = std::make_unique<lc_gui::TextElement>(font, formattedName, "project-text", false, false);
        f.container->addChild(f.text.get());

        f.removeBtn = std::make_unique<lc_gui::ButtonElement>("project-remove", "btn-click", removeFileCallback);
        f.container->addChild(f.removeBtn.get());

        f.removeIcon = std::make_unique<lc_gui::ImageElement>(removeIconTexture.get(), false, "btn-icon");
        f.removeBtn->addChild(f.removeIcon.get());

        f.removeBtn->id = index;

        index++;
    }
}