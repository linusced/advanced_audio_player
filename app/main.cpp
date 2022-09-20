#include "draw_loops/draw_loops.hpp"
#include "main/main.hpp"

enum LoopType
{
    LOOP_TYPE_MENU,
    LOOP_TYPE_MAIN
};

int main(int argc, char const *argv[])
{
    lc_gui::init(true);
    {
        std::string assets = getAssetsFilePath(), appSupport = getApplicationSupportFilePath("Advanced Audio Player");

        lc_gui::Layout windowLayout{-1, -1, lc_gui::Window::getMonitorWidth(), lc_gui::Window::getMonitorHeight()};
        window_position::load(appSupport, &windowLayout);

        lc_gui::Window window{windowLayout.x, windowLayout.y, windowLayout.width, windowLayout.height, "Advanced Audio Player"};
        lc_gui::enableBlend();

        bool devMode = false;
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

        FileExplorer fileExplorer{{{".wav", musicFileIcon.get()}, {".aif", musicFileIcon.get()}}, folderIcon.get(), getDefaultFilePaths(), lc_gui::textFileData(assets + "css/file_explorer.css"), &Arial, &window};
        fileExplorer.resize();

        LoopType drawLoopType = devMode ? LOOP_TYPE_MAIN : LOOP_TYPE_MENU;

        while (!window.shouldClose())
        {
            switch (drawLoopType)
            {
            case LOOP_TYPE_MENU:
            {
                MenuLoop menu{assets, appSupport, &Arial, &fileExplorer, &window};
                menu.loop();
                drawLoopType = LOOP_TYPE_MAIN;
            }
            break;
            case LOOP_TYPE_MAIN:
            {
                MainLoop main{appSupport + "project-1", devMode, &fileExplorer, assets, appSupport, &Arial, &window};
                main.loop();
                drawLoopType = LOOP_TYPE_MENU;
            }
            break;
            }
        }

        window_position::save(appSupport, &window);
    }
    lc_gui::terminate();
    return 0;
}