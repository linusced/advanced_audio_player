#pragma once

#include "lc_gui/lc_gui.hpp"
#include "lc_utility/lc_utility.hpp"
#include "file_explorer/file_explorer.hpp"

class MenuLoop : public lc_gui::DrawLoop
{
public:
    MenuLoop(const lc_utility::FilePath *assetsFilePath, const lc_utility::FilePath *appSupportFilePath, lc_utility::FilePath *projectPath, lc_gui::TextRenderer::Font *font, FileExplorer *fileExplorer, lc_gui::Window *window);
    ~MenuLoop();

    void loop() override;

private:
    const lc_utility::FilePath *assetsFilePath, *appSupportFilePath;
    lc_utility::FilePath *projectPath;
    lc_gui::TextRenderer::Font *font;
    FileExplorer *fileExplorer;

    std::unique_ptr<lc_gui::Texture> loadIconTexture, removeIconTexture;

    std::unique_ptr<lc_gui::TextElement> titleElement, newProjectBtnText;
    std::unique_ptr<lc_gui::Element> mainContentContainer, projectsContainer, newProjectContainer, newProjectFilesContainer;
    std::unique_ptr<lc_gui::ButtonElement> newProjectBtn;

    std::unique_ptr<lc_gui::TextElement> newProjectNameInputLabel, newProjectCancelBtnText, newProjectCreateBtnText, newProjectAddFileBtnText;
    std::unique_ptr<lc_gui::TextInputElement> newProjectNameInput;
    std::unique_ptr<lc_gui::ButtonElement> newProjectCancelBtn, newProjectCreateBtn, newProjectAddFileBtn;

    std::function<void(lc_gui::ButtonElement *)> loadBtnCallback, removeBtnCallback, removeFileCallback;

    struct Project
    {
        std::string name;
        std::unique_ptr<lc_gui::Element> container;
        std::unique_ptr<lc_gui::TextElement> text;
        std::unique_ptr<lc_gui::ButtonElement> loadBtn, removeBtn;
        std::unique_ptr<lc_gui::ImageElement> loadIcon, removeIcon;
    };
    std::vector<Project> projectsArr;

    int loadProjectIndex{-1}, removeProjectIndex{-1};

    struct File
    {
        std::string absPath;
        std::unique_ptr<lc_gui::Element> container;
        std::unique_ptr<lc_gui::TextElement> text;
        std::unique_ptr<lc_gui::ButtonElement> removeBtn;
        std::unique_ptr<lc_gui::ImageElement> removeIcon;
    };
    std::vector<File> newProjectFilesArr;

    int removeFileIndex{-1};

    bool createNewProject{false};

    void updateProjects();
    void updateNewProjectFiles(const std::vector<std::string> &files);
};