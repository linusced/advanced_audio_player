#include "draw_loops/draw_loops.hpp"
#include "config.hpp"

enum LoopType
{
    LOOP_TYPE_MENU,
    LOOP_TYPE_MAIN
};

int main(int argc, char const *argv[])
{
    lc_gui::init(true);
    {
        const lc_utility::FilePath assets = lc_utility::getAssetsFilePath(), appSupport = lc_utility::getApplicationSupportFilePath(PROJECT_NAME.data());

        std::unique_ptr<lc_gui::Window> window;
        {
            lc_gui::Layout windowLayout{-1, -1, lc_gui::Window::getMonitorWidth(), lc_gui::Window::getMonitorHeight()};
            lc_utility::KeyValueFileRead fileIn{appSupport + "window_layout.dat"};

            auto find = fileIn.find("x", lc_utility::KeyValueFile::DATA_INT);
            if (find != fileIn.end())
                find->second.get(windowLayout.x);

            find = fileIn.find("y", lc_utility::KeyValueFile::DATA_INT);
            if (find != fileIn.end())
                find->second.get(windowLayout.y);

            find = fileIn.find("width", lc_utility::KeyValueFile::DATA_INT);
            if (find != fileIn.end())
                find->second.get(windowLayout.width);

            find = fileIn.find("height", lc_utility::KeyValueFile::DATA_INT);
            if (find != fileIn.end())
                find->second.get(windowLayout.height);

            window = std::make_unique<lc_gui::Window>(windowLayout.x, windowLayout.y, windowLayout.width, windowLayout.height, PROJECT_NAME.data());
        }

        lc_gui::enableBlend();

        bool devMode{false};
        for (int i = 1; i < argc; i++)
            if (argv[i] == std::string("-dev"))
                devMode = true;
            else if (argv[i] == std::string("-novsync"))
                glfwSwapInterval(0);

        lc_gui::TextRenderer::Font Arial{assets + "fonts/Arial.ttf"};

        std::unique_ptr<lc_gui::Texture> folderIcon, musicFileIcon;
        {
            lc_gui::TextureData folderIconData(assets + "icons/folder.png"),
                musicFileIconData(assets + "icons/music_file.png");

            folderIcon = std::make_unique<lc_gui::Texture>(folderIconData.bytes, folderIconData.width, folderIconData.height, folderIconData.format);
            musicFileIcon = std::make_unique<lc_gui::Texture>(musicFileIconData.bytes, musicFileIconData.width, musicFileIconData.height, musicFileIconData.format);
        }

        FileExplorer fileExplorer{{{".wav", musicFileIcon.get()}, {".aif", musicFileIcon.get()}}, folderIcon.get(), lc_utility::getDefaultFilePaths(), lc_gui::textFileData(assets + "css/file_explorer.css"), &Arial, window.get()};
        fileExplorer.resize();

        lc_utility::FilePath projectPath = appSupport + "projects";

        LoopType drawLoopType = devMode ? LOOP_TYPE_MAIN : LOOP_TYPE_MENU;

        if (devMode)
            projectPath += "dev";

        while (!window->shouldClose())
        {
            switch (drawLoopType)
            {
            case LOOP_TYPE_MENU:
            {
                MenuLoop menu{&assets, &appSupport, &projectPath, &Arial, &fileExplorer, window.get()};
                menu.loop();
                drawLoopType = LOOP_TYPE_MAIN;
            }
            break;
            case LOOP_TYPE_MAIN:
            {
                MainLoop main{projectPath, devMode, &fileExplorer, std::string(assets) + '/', std::string(appSupport) + '/', &Arial, window.get()}; // update to use lc_utility::FilePath
                main.loop();
                drawLoopType = LOOP_TYPE_MENU;
                projectPath = appSupport + "projects";
            }
            break;
            }
        }

        lc_utility::KeyValueFileWrite fileOut{appSupport + "window_layout.dat"};

        fileOut.push_back("x", window->x());
        fileOut.push_back("y", window->y());
        fileOut.push_back("width", window->width());
        fileOut.push_back("height", window->height());
    }
    lc_gui::terminate();
    return 0;
}