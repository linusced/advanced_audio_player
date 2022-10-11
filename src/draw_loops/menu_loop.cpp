#include "draw_loops/menu_loop.hpp"

MenuLoop::MenuLoop(const lc_utility::FilePath *assetsFilePath, const lc_utility::FilePath *appSupportFilePath, lc_utility::FilePath *projectPath, lc_gui::TextRenderer::Font *font, FileExplorer *fileExplorer, lc_gui::Window *window)
    : lc_gui::DrawLoop(window, lc_gui::textFileData(*assetsFilePath + "css/main_menu.css")),
      assetsFilePath(assetsFilePath), appSupportFilePath(appSupportFilePath), projectPath(projectPath), font(font), fileExplorer(fileExplorer)
{
    lc_gui::TextureData loadIconData{*assetsFilePath + "icons/play.png"}, removeIconData{*assetsFilePath + "icons/remove.png"};
    loadIconTexture = std::make_unique<lc_gui::Texture>(loadIconData.bytes, loadIconData.width, loadIconData.height, loadIconData.format);
    removeIconTexture = std::make_unique<lc_gui::Texture>(removeIconData.bytes, removeIconData.width, removeIconData.height, removeIconData.format);

    titleElement = std::make_unique<lc_gui::TextElement>(font, "Advanced Audio Player", "title", false, false);
    guiRenderer->addChild(titleElement.get());

    mainContentContainer = std::make_unique<lc_gui::Element>("content-container");
    guiRenderer->addChild(mainContentContainer.get());

    projectsContainer = std::make_unique<lc_gui::Element>("projects-arr-container");
    mainContentContainer->addChild(projectsContainer.get());

    newProjectBtn = std::make_unique<lc_gui::ButtonElement>("new-project-btn", "btn-click-light",
                                                            [this](lc_gui::ButtonElement *btn)
                                                            {mainContentContainer->isVisible=false;
                                                            newProjectContainer->isVisible=true; });
    mainContentContainer->addChild(newProjectBtn.get());

    newProjectBtnText = std::make_unique<lc_gui::TextElement>(font, "Create a new project", "btn-text btn-text-dark", false, false);
    newProjectBtn->addChild(newProjectBtnText.get());

    loadBtnCallback = [this](lc_gui::ButtonElement *btn)
    {
        loadProjectIndex = btn->id[0];
    };
    removeBtnCallback = [this](lc_gui::ButtonElement *btn)
    {
        removeProjectIndex = btn->id[0];
    };

    updateProjects();

    newProjectContainer = std::make_unique<lc_gui::Element>("content-container new-project-container");
    newProjectContainer->isVisible = false;
    guiRenderer->addChild(newProjectContainer.get());

    newProjectNameInputLabel = std::make_unique<lc_gui::TextElement>(font, "New project name:", "new-project-name-input-label", false, false);
    newProjectContainer->addChild(newProjectNameInputLabel.get());

    newProjectNameInput = std::make_unique<lc_gui::TextInputElement>(font, "", "new-project-name-input",
                                                                     [this](lc_gui::TextInputElement *input, std::string &str)
                                                                     {
                                                                         newProjectCreateBtn->isVisible = !str.empty() && !newProjectFilesArr.empty();
                                                                     });
    newProjectContainer->addChild(newProjectNameInput.get());

    newProjectCancelBtn = std::make_unique<lc_gui::ButtonElement>("new-project-cancel-btn", "new-project-cancel-btn-click",
                                                                  [this](lc_gui::ButtonElement *btn)
                                                                  {
                                                                      mainContentContainer->isVisible = true;
                                                                      newProjectContainer->isVisible = false;
                                                                      newProjectNameInput->setText("");
                                                                      updateNewProjectFiles({});
                                                                  });
    newProjectContainer->addChild(newProjectCancelBtn.get());

    newProjectCancelBtnText = std::make_unique<lc_gui::TextElement>(font, "Cancel", "btn-text", false, false);
    newProjectCancelBtn->addChild(newProjectCancelBtnText.get());

    newProjectCreateBtn = std::make_unique<lc_gui::ButtonElement>("new-project-create-btn", "new-project-create-btn-click",
                                                                  [this](lc_gui::ButtonElement *btn)
                                                                  { createNewProject = true; });
    newProjectCreateBtn->isVisible = false;
    newProjectContainer->addChild(newProjectCreateBtn.get());

    newProjectCreateBtnText = std::make_unique<lc_gui::TextElement>(font, "Create", "btn-text", false, false);
    newProjectCreateBtn->addChild(newProjectCreateBtnText.get());

    newProjectAddFileBtn = std::make_unique<lc_gui::ButtonElement>("new-project-add-file-btn", "btn-click-light", [this](lc_gui::ButtonElement *btn)
                                                                   { this->fileExplorer->open(newProjectFilesArr.empty() ? this->fileExplorer->getDefaultFolders().at("Music") : newProjectFilesArr.back().absPath); });
    newProjectContainer->addChild(newProjectAddFileBtn.get());

    newProjectAddFileBtnText = std::make_unique<lc_gui::TextElement>(font, "Add file", "btn-text btn-text-dark", false, false);
    newProjectAddFileBtn->addChild(newProjectAddFileBtnText.get());

    newProjectFilesContainer = std::make_unique<lc_gui::Element>("new-project-files");
    newProjectContainer->addChild(newProjectFilesContainer.get());

    removeFileCallback = [this](lc_gui::ButtonElement *btn)
    {
        removeFileIndex = btn->id[0];
    };
}
MenuLoop::~MenuLoop()
{
    newProjectNameInput->stopThread();
}

void MenuLoop::loop()
{
    bool resizeInit{true}, keyDown{false};

    while (!window->shouldClose())
    {
        window->update();

        if (window->hasResized() || resizeInit)
        {
            resizeInit = false;
            guiRenderer->resize();
            fileExplorer->resize();
        }

        if (createNewProject)
        {
            *projectPath += newProjectNameInput->getText();

            std::filesystem::create_directory(std::string(*projectPath));
            std::ofstream fileOut{*projectPath + "paths.dat"};

            for (size_t i = 0; i < newProjectFilesArr.size(); i++)
            {
                if (i != 0)
                    fileOut << '\n';

                fileOut << newProjectFilesArr[i].absPath;
            }
            return;
        }
        else if (loadProjectIndex != -1)
        {
            *projectPath += projectsArr[loadProjectIndex].name;
            return;
        }
        else if (removeProjectIndex != -1)
        {
            std::filesystem::remove_all(std::string(*projectPath + projectsArr[removeProjectIndex].name));
            updateProjects();
            removeProjectIndex = -1;
        }
        else if (removeFileIndex != -1)
        {
            size_t size = newProjectFilesArr.size();
            std::vector<std::string> files(size - 1);

            for (size_t i = 0, j = 0; i < size; i++)
                if (i != removeFileIndex)
                {
                    files[j] = newProjectFilesArr[i].absPath;
                    j++;
                }

            updateNewProjectFiles(files);

            newProjectCreateBtn->isVisible = !newProjectNameInput->getText().empty() && !newProjectFilesArr.empty();
            removeFileIndex = -1;
        }

        if (fileExplorer->isActive())
        {
            fileExplorer->update();

            if (fileExplorer->userHasSelectedAFile())
            {
                size_t size = newProjectFilesArr.size();
                std::vector<std::string> files(size + 1);

                for (size_t i = 0; i < size; i++)
                    files[i] = newProjectFilesArr[i].absPath;

                files.back() = fileExplorer->getSelectedFilePath();

                updateNewProjectFiles(files);

                newProjectCreateBtn->isVisible = !newProjectNameInput->getText().empty() && !newProjectFilesArr.empty();
                guiRenderer->update();
            }
        }
        else
            guiRenderer->update();

        glClear(GL_COLOR_BUFFER_BIT);

        guiRenderer->draw();

        if (fileExplorer->isActive())
            fileExplorer->draw();

        window->swapBuffers();
    }
}