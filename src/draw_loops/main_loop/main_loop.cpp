#include "draw_loops/main_loop.hpp"

const int MainLoop::WAVEFORM_HEIGHT{150};

MainLoop::MainLoop(std::string projectFolderPath, bool devMode, FileExplorer *fileExplorer, std::string assetsFilePath, std::string appSupportFilePath, lc_gui::TextRenderer::Font *font, lc_gui::Window *window)
    : lc_gui::DrawLoop(window, lc_gui::textFileData(assetsFilePath + "css/main.css") + lc_gui::textFileData(assetsFilePath + "css/waveform_data.css") + lc_gui::textFileData(assetsFilePath + "css/main_controls.css") + lc_gui::textFileData(assetsFilePath + "css/keyframe_editor.css") + lc_gui::textFileData(assetsFilePath + "css/file_manager.css")),
      devMode(devMode), fileExplorer(fileExplorer), assetsFilePath(assetsFilePath), appSupportFilePath(appSupportFilePath), font(font), keyHandler(window)
{
    audioPlayer = std::make_unique<AdvancedAudioPlayer>(projectFolderPath);

    {
        std::ifstream slidersFile(appSupportFilePath + "sliders.dat");
        if (slidersFile.is_open())
        {
            std::string line, key;
            float value;
            size_t spaceIndex;

            while (!slidersFile.eof())
            {
                std::getline(slidersFile, line);
                try
                {
                    spaceIndex = line.find(' ');
                    if (spaceIndex != std::string::npos)
                    {
                        key = line.substr(0, spaceIndex);
                        value = std::stof(line.substr(spaceIndex + 1));

                        if (key == "volume")
                            audioPlayer->setVolume(value);
                        else if (key == "metronome")
                            audioPlayer->setMetronomeVolume(value);
                    }
                }
                catch (...)
                {
                    continue;
                }
            }
        }
    }

    // waveform
    waveformContainer = std::make_unique<lc_gui::Element>("waveform-container");
    guiRenderer->addChild(waveformContainer.get());

    waveformElement = std::make_unique<audio_player_gui::Waveform>(lc_gui::Window::getMonitorWidth(), 300, audioPlayer->NUM_CHANNELS, audioPlayer->SAMPLE_RATE, "waveform");

    createCallbacks();

    waveformLmbTypeContainer = std::make_unique<lc_gui::Element>("waveform-lmb-type");
    guiRenderer->addChild(waveformLmbTypeContainer.get());

    waveformLmbTypeText = std::make_unique<lc_gui::TextElement>(font, "Click type", "waveform-lmb-type-text", true, false);
    waveformLmbTypeContainer->addChild(waveformLmbTypeText.get());

    waveformY = &waveformContainer->elementStyle.sizeProperties["top"];
    waveformLmbY = &waveformLmbTypeContainer->elementStyle.sizeProperties["top"];
    waveformY->isSet = waveformLmbY->isSet = true;
    waveformY->unit = waveformLmbY->unit = lc_gui::Style::PIXELS;
    waveformY->value = waveformLmbY->value = 0;

    // scroll
    waveformScrollbar = std::make_unique<audio_player_gui::Scrollbar>(0, WAVEFORM_HEIGHT, 0, 0, waveformScrollCallback, "waveform-scrollbar");
    guiRenderer->addChild(waveformScrollbar.get());
    waveformScrollbar->elementStyle.sizeProperties["height"].isSet = true;
    waveformScrollbar->elementStyle.sizeProperties["height"].unit = lc_gui::Style::PIXELS;

    createWaveformLmbTypeGUI();
    createMainControlsGUI();
    createKeyframeEditorGUI();
    createLoadingGUI();

    fileManagerContainer = std::make_unique<lc_gui::Element>("file-manager");
    fileManagerContainer->isVisible = false;
    guiRenderer->addChild(fileManagerContainer.get());

    auto &bottom = fileManagerContainer->elementStyle.sizeProperties["bottom"];
    bottom.isSet = true;
    bottom.unit = lc_gui::Style::PIXELS;
    bottom.value = guiRenderer->getStyle("main-controls")->sizeProperties.at("height").value;

    fileManagerResetBtn = std::make_unique<lc_gui::ButtonElement>(fileManagerBtnCallback, "file-manager-reset", "file-manager-reset-click");
    fileManagerResetBtn->id = "reset-0";

    fileManagerResetText = std::make_unique<lc_gui::TextElement>(font, "Reset", "btn-text", false, false);
    fileManagerResetBtn->addChild(fileManagerResetText.get());

    fileManagerSaveBtn = std::make_unique<lc_gui::ButtonElement>(fileManagerBtnCallback, "file-manager-save", "file-manager-save-click");
    fileManagerSaveBtn->id = "save-0";

    fileManagerSaveText = std::make_unique<lc_gui::TextElement>(font, "Save", "btn-text", false, false);
    fileManagerSaveBtn->addChild(fileManagerSaveText.get());

    fileManagerAddBtn = std::make_unique<lc_gui::ButtonElement>(fileManagerBtnCallback, "file-manager-add", "file-manager-add-click");
    fileManagerAddBtn->id = "add-0";

    fileManagerAddText = std::make_unique<lc_gui::TextElement>(font, "Open new file", "btn-text", false, false);
    fileManagerAddBtn->addChild(fileManagerAddText.get());

    lc_gui::TextureData moveDownIconData{assetsFilePath + "icons/move_down.png"};
    moveDownIcon = std::make_unique<lc_gui::Texture>(moveDownIconData.bytes, moveDownIconData.width, moveDownIconData.height, moveDownIconData.format);

    lc_gui::TextureData moveUpIconData{assetsFilePath + "icons/move_up.png"};
    moveUpIcon = std::make_unique<lc_gui::Texture>(moveUpIconData.bytes, moveUpIconData.width, moveUpIconData.height, moveUpIconData.format);

    lc_gui::TextureData removeIconData{assetsFilePath + "icons/remove.png"};
    removeIcon = std::make_unique<lc_gui::Texture>(removeIconData.bytes, removeIconData.width, removeIconData.height, removeIconData.format);

    prevVolumeValue = volumeSlider->getValue();
    if (prevVolumeValue == 0.0f)
        prevVolumeValue = 0.2f;

    prevMetronomeValue = metronomeSlider->getValue();
    if (prevMetronomeValue == 0.0f)
        prevMetronomeValue = 0.2f;

    createKeyHandler();
}

MainLoop::~MainLoop()
{
    std::ofstream slidersFile(appSupportFilePath + "sliders.dat");
    if (slidersFile.is_open())
    {
        slidersFile << "volume " << audioPlayer->getVolume() << '\n'
                    << "metronome " << audioPlayer->getMetronomeVolume();
    }

    for (auto &w : waveformDataArr)
    {
        w.bpmInput->stopThread();
        w.beatOffsetInput->stopThread();
        w.pitchChangeBpmInput->stopThread();
    }

    keyframeEditorAmplitudeInput->stopThread();
    keyframeEditorLowPassFrequencyInput->stopThread();
    keyframeEditorHighPassFrequencyInput->stopThread();
}

void MainLoop::waveformScrollbarResize()
{
    int mainControlsY = mainControls->getPreviousLayout().y,
        fullWaveformsVisible = mainControlsY / WAVEFORM_HEIGHT;
    if (fullWaveformsVisible == 0)
        fullWaveformsVisible = 1;

    waveformScrollbar->setHeight(fullWaveformsVisible * WAVEFORM_HEIGHT);
    waveformScrollbar->setValue(0);

    int containerHeight = waveformContainer->getPreviousLayout().height;
    waveformScrollbar->elementStyle.sizeProperties["height"].value = containerHeight;

    if (containerHeight > mainControlsY)
        waveformScrollbar->setMax(WAVEFORM_HEIGHT * waveformDataArr.size());
    else
        waveformScrollbar->setMax(0);
}