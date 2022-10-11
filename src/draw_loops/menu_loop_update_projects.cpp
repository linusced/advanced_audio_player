#include "draw_loops/menu_loop.hpp"

void MenuLoop::updateProjects()
{
    projectsContainer->children.clear();
    projectsArr.clear();
    projectsContainer->styleChange = true;

    std::string projectsDir = *projectPath;

    if (std::filesystem::is_directory(projectsDir))
    {
        int index{0},
            yOffset = guiRenderer->getStyle("project-container")->sizeProperties.at("height").value + guiRenderer->getStyle("project-container")->sizeProperties.at("border-width").value * 2;

        auto dirIt = std::filesystem::directory_iterator(projectsDir);
        for (auto &project : dirIt)
            if (project.is_directory())
            {
                projectsArr.push_back({});
                auto &p = projectsArr.back();
                p.name = project.path().filename();

                p.container = std::make_unique<lc_gui::Element>("project-container");
                auto &y = p.container->elementStyle.sizeProperties["top"];
                y.isSet = true;
                y.unit = lc_gui::Style::PIXELS;
                y.value = index * yOffset;
                projectsContainer->addChild(p.container.get());

                std::string formattedName = p.name;
                formattedName[0] = std::toupper(formattedName[0]);

                for (size_t i = 1; i < formattedName.size() - 1; i++)
                    if (formattedName[i] == '-' || formattedName[i] == '_')
                    {
                        formattedName[i] = ' ';
                        formattedName[i + 1] = std::toupper(formattedName[i + 1]);
                    }

                p.text = std::make_unique<lc_gui::TextElement>(font, formattedName, "project-text", false, false);
                p.container->addChild(p.text.get());

                p.loadBtn = std::make_unique<lc_gui::ButtonElement>("project-load", "btn-click", loadBtnCallback);
                p.container->addChild(p.loadBtn.get());
                p.loadIcon = std::make_unique<lc_gui::ImageElement>(loadIconTexture.get(), false, "btn-icon");
                p.loadBtn->addChild(p.loadIcon.get());

                p.removeBtn = std::make_unique<lc_gui::ButtonElement>("project-remove", "btn-click", removeBtnCallback);
                p.container->addChild(p.removeBtn.get());
                p.removeIcon = std::make_unique<lc_gui::ImageElement>(removeIconTexture.get(), false, "btn-icon");
                p.removeBtn->addChild(p.removeIcon.get());

                p.loadBtn->id = p.removeBtn->id = index;

                index++;
            }
    }
    else
        std::filesystem::create_directory(projectsDir);
}