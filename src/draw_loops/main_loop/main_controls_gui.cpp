#include "draw_loops/main_loop.hpp"

void MainLoop::createMainControlsGUI()
{
    mainControls = std::make_unique<lc_gui::Element>("main-controls");
    guiRenderer->addChild(mainControls.get());

    mainControlsBtns = std::make_unique<lc_gui::Element>("main-controls-btns");
    mainControls->addChild(mainControlsBtns.get());

    togglePlaybackBtn = std::make_unique<lc_gui::ButtonElement>(mainControlsBtnCallback, "toggle-playback-btn", "btn-click-light");
    togglePlaybackBtn->id = "toggle-playback";
    mainControlsBtns->addChild(togglePlaybackBtn.get());

    togglePlaybackText = std::make_unique<lc_gui::TextElement>(font, "Play", "btn-text btn-text-dark", false, false);
    togglePlaybackBtn->addChild(togglePlaybackText.get());

    saveProjectBtn = std::make_unique<lc_gui::ButtonElement>(mainControlsBtnCallback, "save-project-btn", "btn-click-light");
    saveProjectBtn->id = "save-project";
    mainControlsBtns->addChild(saveProjectBtn.get());

    saveProjectText = std::make_unique<lc_gui::TextElement>(font, "Save project", "btn-text btn-text-dark", false, false);
    saveProjectBtn->addChild(saveProjectText.get());

    bounceProjectBtn = std::make_unique<lc_gui::ButtonElement>(mainControlsBtnCallback, "bounce-project-btn", "btn-click-light");
    bounceProjectBtn->id = "bounce-project";
    mainControlsBtns->addChild(bounceProjectBtn.get());

    bounceProjectText = std::make_unique<lc_gui::TextElement>(font, "Bounce project", "btn-text btn-text-dark", false, false);
    bounceProjectBtn->addChild(bounceProjectText.get());

    toggleFileManagerBtn = std::make_unique<lc_gui::ButtonElement>(mainControlsBtnCallback, "toggle-file-manager-btn", "btn-click");
    toggleFileManagerBtn->id = "toggle-file-manager";
    mainControlsBtns->addChild(toggleFileManagerBtn.get());

    toggleFileManagerText = std::make_unique<lc_gui::TextElement>(font, "Show file manager", "btn-text", false, false);
    toggleFileManagerBtn->addChild(toggleFileManagerText.get());

    auto *width = &mainControlsBtns->elementStyle.sizeProperties["width"];
    width->isSet = true;
    width->unit = lc_gui::Style::PIXELS;
    width->value = guiRenderer->getStyle("toggle-playback-btn")->sizeProperties.at("width").value + 5.0f +
                   guiRenderer->getStyle("save-project-btn")->sizeProperties.at("width").value + 5.0f +
                   guiRenderer->getStyle("bounce-project-btn")->sizeProperties.at("width").value + 5.0f +
                   guiRenderer->getStyle("toggle-file-manager-btn")->sizeProperties.at("width").value;

    mainControlsSliders = std::make_unique<lc_gui::Element>("main-controls-sliders");
    mainControls->addChild(mainControlsSliders.get());

    volumeLabel = std::make_unique<lc_gui::TextElement>(font, "Volume:", "volume-label", false, false);
    mainControlsSliders->addChild(volumeLabel.get());

    volumeSlider = std::make_unique<lc_gui::SliderElement>(0.0f, 1.0f, audioPlayer->getVolume(), mainControlsSliderCallback, "volume-slider", "volume-slider-mouse-down");
    volumeSlider->callbackOnInput = true;
    volumeSlider->id = "volume";
    mainControlsSliders->addChild(volumeSlider.get());

    metronomeLabel = std::make_unique<lc_gui::TextElement>(font, "Metronome:", "metronome-label", false, false);
    mainControlsSliders->addChild(metronomeLabel.get());

    metronomeSlider = std::make_unique<lc_gui::SliderElement>(0.0f, 1.0f, audioPlayer->getMetronomeVolume(), mainControlsSliderCallback, "metronome-slider", "metronome-slider-mouse-down");
    metronomeSlider->callbackOnInput = true;
    metronomeSlider->id = "metronome";
    mainControlsSliders->addChild(metronomeSlider.get());

    width = &mainControlsSliders->elementStyle.sizeProperties["width"];
    width->isSet = true;
    width->unit = lc_gui::Style::PIXELS;
    width->value = guiRenderer->getStyle("volume-label")->sizeProperties.at("width").value +
                   guiRenderer->getStyle("volume-slider")->sizeProperties.at("width").value + 40.0f +
                   guiRenderer->getStyle("metronome-label")->sizeProperties.at("width").value +
                   guiRenderer->getStyle("metronome-slider")->sizeProperties.at("width").value + 40.0f;

    width = &mainControls->elementStyle.sizeProperties["width"];
    width->isSet = true;
    width->unit = lc_gui::Style::PIXELS;
    width->value = mainControlsBtns->elementStyle.sizeProperties["width"].value = std::max(mainControlsBtns->elementStyle.sizeProperties["width"].value, mainControlsSliders->elementStyle.sizeProperties["width"].value);
}