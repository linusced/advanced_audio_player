#include "file_explorer/file_explorer.hpp"

void FileExplorer::updateDirectories()
{
    dirPathUpdate = false;
    if (dirPath.back() != '/')
        dirPath.push_back('/');

    updateCurrentPath(dirPath);

    directoriesContainer->children.clear();
    directoriesContainer->styleChange = true;
    directories.clear();

    goBtn->isVisible = false;
    prevClickIndex = -1;

    for (auto &d : defaultFoldersDirectories)
    {
        if (dirPath == defaultFolders[d.name])
            d.btn->classList.add("directory-btn-active");
        else
            d.btn->classList.remove("directory-btn-active");

        d.btn->styleChange = true;
    }

    float btnHeight = renderer->getStyle("directory-btn")->sizeProperties.at("height").value;
    std::string textOutput;

    std::vector<std::filesystem::directory_entry> dirEntries;

    auto it = std::filesystem::directory_iterator(dirPath);
    for (auto &i : it)
        if (i.is_directory() || fileTypes.find(i.path().extension()) != fileTypes.end())
            dirEntries.push_back(i);

    std::sort(dirEntries.begin(), dirEntries.end(), [](const std::filesystem::directory_entry &dir1, const std::filesystem::directory_entry &dir2)
              {
                if(dir1.is_directory() && !dir2.is_directory())
                    return true;
                if (dir2.is_directory() && !dir1.is_directory())
                    return false;

                std::string str1{dir1.path().extension().string()}, str2{dir2.path().extension().string()};
                
                if((dir1.is_directory() && dir2.is_directory()) || (str1 == str2))
                {
                    str1 = dir1.path().filename().string();
                    str2 = dir2.path().filename().string();
                }

                size_t minSize = str1.size() < str2.size() ? str1.size() : str2.size();
                int num1, num2;

                for (size_t i = 0; i < minSize; i++)
                {
                    num1 = tolower(str1[i]);
                    num2 = tolower(str2[i]);
                    if (num1 != num2)
                        return num1 < num2;
                }
                return false; });

    size_t size = dirEntries.size();
    directories.resize(size);

    for (size_t i = 0; i < size; i++)
    {
        auto &d = directories[i];

        d.name = dirEntries[i].path().filename();
        d.isFolder = dirEntries[i].is_directory();

        d.btn = std::make_unique<lc_gui::ButtonElement>(directoryBtnCallback, "directory-btn", "directory-btn-click");
        d.btn->id.push_back(i);
        directoriesContainer->addChild(d.btn.get());

        auto &y = d.btn->elementStyle.sizeProperties["top"];
        y.isSet = true;
        y.unit = lc_gui::Style::PIXELS;
        y.value = i * btnHeight;

        d.icon = std::make_unique<lc_gui::ImageElement>(d.isFolder ? folderIconTexture : fileTypes[dirEntries[i].path().extension()], false, "directory-icon");
        d.btn->addChild(d.icon.get());

        if (!textOutput.empty())
            textOutput.push_back('\n');

        textOutput += d.name;
    }

    for (size_t i = 0; i < textOutput.size(); i++)
        if (!isascii(textOutput[i]))
        {
            textOutput.erase(textOutput.begin() + i);
            i--;
        }

    directoriesContainer->addChild(directoryText.get());
    directoryText->setText(textOutput);

    directoriesContainer->addChild(directoriesContainerScrollBG.get());
}