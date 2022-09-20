#include "draw_loops/main_loop.hpp"

void MainLoop::createLoadingGUI()
{
    loadingGUI = std::make_unique<lc_gui::Renderer>(lc_gui::textFileData(assetsFilePath + "css/loading.css"), window);

    loadingFullscreenContainer = std::make_unique<lc_gui::Element>("fullscreen-container");
    loadingGUI->addChild(loadingFullscreenContainer.get());

    loadingContainer = std::make_unique<lc_gui::Element>("container");
    loadingGUI->addChild(loadingContainer.get());

    loadingText = std::make_unique<lc_gui::TextElement>(font, "Loading project files...", "text", false, false);
    loadingContainer->addChild(loadingText.get());

    savingText = std::make_unique<lc_gui::TextElement>(font, "Saving project files...", "text", false, false);
    savingText->isVisible = false;
    loadingContainer->addChild(savingText.get());

    pitchChangeText = std::make_unique<lc_gui::TextElement>(font, "Changing file pitch...", "text", false, false);
    pitchChangeText->isVisible = false;
    loadingContainer->addChild(pitchChangeText.get());

    bounceText = std::make_unique<lc_gui::TextElement>(font, "Bouncing project...", "text", false, false);
    bounceText->isVisible = false;
    loadingContainer->addChild(bounceText.get());

    loadingProgressContainer = std::make_unique<lc_gui::Element>("progress-container");
    loadingContainer->addChild(loadingProgressContainer.get());

    loadingProgress = std::make_unique<lc_gui::Element>("progress");
    loadingProgressContainer->addChild(loadingProgress.get());

    loadingProgressWidth = &loadingProgress->elementStyle.sizeProperties["width"];
    loadingProgressWidth->isSet = true;
    loadingProgressWidth->unit = lc_gui::Style::PERCENTAGE;
    loadingProgressWidth->value = 0.0f;
}