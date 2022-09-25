#include "file_explorer/file_explorer.hpp"

const double FileExplorer::DOUBLE_CLICK_MAX_INTERVAL{0.5};

FileExplorer::FileExplorer(std::unordered_map<std::string, lc_gui::Texture *> fileTypes, lc_gui::Texture *_folderIconTexture, std::map<std::string, std::string> _defaultFolders, std::string css, lc_gui::TextRenderer::Font *font, lc_gui::Window *window)
    : fileTypes(fileTypes), folderIconTexture(_folderIconTexture), defaultFolders(_defaultFolders), font(font), window(window), renderer(std::make_unique<lc_gui::Renderer>(css, window))
{
    createCallbacks();

    fullscreenContainer = std::make_unique<lc_gui::Element>("fullscreen-container");
    renderer->addChild(fullscreenContainer.get());

    container = std::make_unique<lc_gui::Element>("container");
    renderer->addChild(container.get());

    directoriesContainer = std::make_unique<lc_gui::Element>("directories-container");
    container->addChild(directoriesContainer.get());

    directoriesContainerScrollBG = std::make_unique<lc_gui::Element>("directory-container-scroll-bg");
    directoryText = std::make_unique<lc_gui::TextElement>(font, "", "directory-text", false, true, false);

    createSidebarGUI();
    createTopbarGUI();
    createBtmbarGUI();
}
FileExplorer::~FileExplorer()
{
    fileNameInput->stopThread();
    directoryText->stopThread();
    currentPath->stopThread();
}

void FileExplorer::resize()
{
    renderer->resize();
}
void FileExplorer::draw()
{
    renderer->draw();
}

void FileExplorer::open(std::string dirPath, bool fileNameInput, std::string fileName)
{
    size_t extensionIndex = dirPath.find_last_of('.');
    if (extensionIndex != std::string::npos)
    {
        std::string extension = dirPath.substr(extensionIndex);
        for (auto [name, texture] : fileTypes)
            if (name == extension)
            {
                dirPath = dirPath.substr(0, dirPath.find_last_of('/'));
                break;
            }
    }

    this->dirPath = dirPath;
    btmbarContent->isVisible = fileNameInput;
    this->fileNameInput->setText(fileName);

    active = true;

    for (auto &k : keyDown)
        k.second = false;

    updateDirectories();
}

const bool &FileExplorer::isActive() const
{
    return active;
}
const bool &FileExplorer::userHasSelectedAFile() const
{
    return fileSelected;
}

std::string FileExplorer::getSelectedFilePath()
{
    fileSelected = active = false;
    directoriesContainer->children.clear();
    directoriesContainer->styleChange = true;
    directories.clear();
    dirPath.clear();
    return selectedFile;
}

const std::map<std::string, std::string> &FileExplorer::getDefaultFolders() const
{
    return defaultFolders;
}

void FileExplorer::updateCurrentPath(std::string str)
{
    constexpr size_t maxSize = 100, dotsSize = 2, maxSizeSubtract = maxSize - dotsSize;
    if (str.size() > maxSize)
        str = std::string(dotsSize, '.') + str.substr(str.find_first_of('/', str.size() - maxSizeSubtract));

    currentPath->setText(str);
}