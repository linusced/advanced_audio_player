#pragma once

#include "lc_gui/lc_gui.hpp"
#include "file_explorer/file_explorer.hpp"

class MenuLoop : public lc_gui::DrawLoop
{
public:
    MenuLoop(std::string assetsFilePath, std::string appSupportFilePath, lc_gui::TextRenderer::Font *font, FileExplorer *fileExplorer, lc_gui::Window *window);

    void loop() override;

private:
    std::string assetsFilePath, appSupportFilePath;
    lc_gui::TextRenderer::Font *font;
    FileExplorer *fileExplorer;
};