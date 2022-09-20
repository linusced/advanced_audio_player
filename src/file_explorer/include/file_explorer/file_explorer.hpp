#pragma once

#include "lc_gui/lc_gui.hpp"

class FileExplorer
{
public:
    FileExplorer(std::unordered_map<std::string, lc_gui::Texture *> fileTypes, lc_gui::Texture *folderIconTexture, std::map<std::string, std::string> defaultFolders, std::string css, lc_gui::TextRenderer::Font *font, lc_gui::Window *window);
    ~FileExplorer();

    void resize();
    void update();
    void draw();

    void open(std::string dirPath, bool fileNameInput = false, std::string fileName = "");

    const bool &isActive() const;
    const bool &userHasSelectedAFile() const;

    std::string getSelectedFilePath();

    const std::map<std::string, std::string> &getDefaultFolders() const;

private:
    struct Directory
    {
        std::string name;
        bool isFolder;

        std::unique_ptr<lc_gui::ButtonElement> btn;
        std::unique_ptr<lc_gui::ImageElement> icon;
    };

    std::unordered_map<std::string, lc_gui::Texture *> fileTypes;
    lc_gui::Texture *folderIconTexture;
    std::map<std::string, std::string> defaultFolders;
    lc_gui::TextRenderer::Font *font;
    lc_gui::Window *window;

    bool active{false}, fileSelected{false};
    std::string selectedFile;

    std::unique_ptr<lc_gui::Renderer> renderer;

    std::unique_ptr<lc_gui::Element> fullscreenContainer, container,
        sidebar, topbar, btmbar, directoriesContainer,
        directoriesContainerScrollBG;

    // directories
    std::unique_ptr<lc_gui::TextElement> directoryText;

    std::vector<Directory> directories;
    std::string dirPath;
    bool dirPathUpdate{false};

    std::function<void(lc_gui::ButtonElement *)> directoryBtnCallback;
    int prevClickIndex{-1};
    double prevClickTime{0.0};
    static const double DOUBLE_CLICK_MAX_INTERVAL;

    // sidebar
    std::unique_ptr<lc_gui::TextElement> defaultFoldersText;
    std::vector<Directory> defaultFoldersDirectories;
    std::function<void(lc_gui::ButtonElement *)> defaultFolderBtnCallback;

    // topbar
    std::function<void(lc_gui::ButtonElement *)> topbarBtnCallback;

    std::unique_ptr<lc_gui::TextElement> currentPath, parentFolderBtnText, goBtnText, cancelBtnText;
    std::unique_ptr<lc_gui::ButtonElement> parentFolderBtn, goBtn, cancelBtn;

    // btmbar
    std::function<void(lc_gui::ButtonElement *)> btmbarSelectCallback;

    std::unique_ptr<lc_gui::Element> btmbarContent;
    std::unique_ptr<lc_gui::TextElement> fileNameInputLabel, btmbarSelectText;
    std::unique_ptr<lc_gui::TextInputElement> fileNameInput;
    std::unique_ptr<lc_gui::ButtonElement> fileNameInputBtn, btmbarSelectBtn;

    bool cancel{false};
    std::unordered_map<GLenum, bool> keyDown;

    void createCallbacks();

    void createSidebarGUI();
    void createTopbarGUI();
    void createBtmbarGUI();

    void updateDirectories();
    void updateCurrentPath(std::string str);
};