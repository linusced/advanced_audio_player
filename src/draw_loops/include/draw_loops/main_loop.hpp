#pragma once

#include "lc_gui/lc_gui.hpp"
#include "advanced_audio_player/advanced_audio_player.hpp"
#include "audio_player_gui/audio_player_gui.hpp"
#include "file_explorer/file_explorer.hpp"

class MainLoop : public lc_gui::DrawLoop
{
public:
    MainLoop(std::string projectFolderPath, bool devMode, FileExplorer *fileExplorer, std::string assetsFilePath, std::string appSupportFilePath, lc_gui::TextRenderer::Font *font, lc_gui::Window *window);
    ~MainLoop() override;

    void loop() override;

private:
    static const int WAVEFORM_HEIGHT;

    bool devMode;
    FileExplorer *fileExplorer;
    std::string assetsFilePath, appSupportFilePath;
    lc_gui::TextRenderer::Font *font;

    std::unique_ptr<AdvancedAudioPlayer> audioPlayer;

    // waveform
    std::unique_ptr<lc_gui::Element> waveformContainer;
    std::unique_ptr<audio_player_gui::Waveform> waveformElement;

    // waveform lmb type
    std::unique_ptr<lc_gui::Element> waveformLmbTypeContainer;
    std::unique_ptr<lc_gui::TextElement> waveformLmbTypeText;

    lc_gui::Style::SIZE *waveformY{nullptr}, *waveformLmbY{nullptr};

    struct WaveformLmbType
    {
        std::unique_ptr<lc_gui::ButtonElement> btn;
        std::unique_ptr<lc_gui::TextElement> text;
    };

    std::function<void(lc_gui::ButtonElement *)> waveformLmbTypeCallback;

    std::vector<WaveformLmbType> waveformLmbTypeArr;

    // waveform scroll
    std::function<void(int)> waveformScrollCallback;
    std::unique_ptr<audio_player_gui::Scrollbar> waveformScrollbar;

    // main controls
    std::function<void(lc_gui::ButtonElement *)> mainControlsBtnCallback;
    std::function<void(lc_gui::SliderElement *, float &)> mainControlsSliderCallback;

    std::unique_ptr<lc_gui::Element> mainControls, mainControlsBtns, mainControlsSliders;
    std::unique_ptr<lc_gui::ButtonElement> togglePlaybackBtn, saveProjectBtn, bounceProjectBtn, toggleFileManagerBtn;
    std::unique_ptr<lc_gui::TextElement> togglePlaybackText, saveProjectText, bounceProjectText, toggleFileManagerText,
        volumeLabel, metronomeLabel;
    std::unique_ptr<lc_gui::SliderElement> volumeSlider, metronomeSlider;

    struct FileManagerItem
    {
        int waveformIndex, initialWaveformIndex{-1};
        std::string filePath;
        std::unique_ptr<lc_gui::Element> container;
        std::unique_ptr<lc_gui::TextElement> name;
        std::unique_ptr<lc_gui::ButtonElement> moveUpBtn, moveDownBtn, removeBtn;
        std::unique_ptr<lc_gui::ImageElement> moveUpBtnIcon, moveDownBtnIcon, removeBtnIcon;
    };

    std::unique_ptr<lc_gui::Texture> moveUpIcon, moveDownIcon, removeIcon;
    std::function<void(lc_gui::ButtonElement *)> fileManagerBtnCallback;

    std::vector<FileManagerItem> fileManagerItemsArr;

    std::unique_ptr<lc_gui::Element> fileManagerContainer;
    std::unique_ptr<lc_gui::ButtonElement> fileManagerResetBtn, fileManagerSaveBtn, fileManagerAddBtn;
    std::unique_ptr<lc_gui::TextElement> fileManagerResetText, fileManagerSaveText, fileManagerAddText;

    bool updateFileManagerNextFrame{false}, updateWaveformDataNextFrame{false},
        openFileExplorer{false}, updateFilesAfterLoading{false}, resetLoading{false};

    // waveform data
    struct WaveformData
    {
        std::unique_ptr<lc_gui::Element> container;
        std::unique_ptr<lc_gui::TextElement> name,
            bpmLabel, beatOffsetLabel, pitchChangeBpmLabel, pitchChangeText, muteBtnText;

        std::unique_ptr<lc_gui::TextInputElement> bpmInput,
            beatOffsetInput, pitchChangeBpmInput;

        std::unique_ptr<lc_gui::ButtonElement> pitchChangeBtn, muteBtn;
        unsigned int pitchBPM;
    };

    std::function<void(lc_gui::TextInputElement *, std::string &)> waveformDataInputCallback;
    std::function<void(lc_gui::ButtonElement *)> waveformDataBtnCallback;

    std::vector<WaveformData> waveformDataArr;

    // keyframe editor
    std::function<void(lc_gui::TextInputElement *, std::string &)> keyframeEditorInputCallback;
    std::function<void(lc_gui::SliderElement *, float &)> keyframeEditorSliderCallback;

    std::unique_ptr<lc_gui::Element> keyframeEditor;
    std::unique_ptr<lc_gui::TextElement> keyframeEditorText,
        keyframeEditorAmplitudeLabel, keyframeEditorLowPassFrequencyLabel, keyframeEditorHighPassFrequencyLabel;
    std::unique_ptr<lc_gui::TextInputElement> keyframeEditorAmplitudeInput, keyframeEditorLowPassFrequencyInput, keyframeEditorHighPassFrequencyInput;
    std::unique_ptr<lc_gui::SliderElement> keyframeEditorAmplitudeSlider, keyframeEditorLowPassFrequencySlider, keyframeEditorHighPassFrequencySlider;

    lc_gui::Style::SIZE *keyframeEditorPosX{nullptr}, *keyframeEditorPosY{nullptr};
    bool displayKeyframEditor{false};

    AdvancedAudioPlayer::Keyframe *currentKeyframe{nullptr};

    // loading GUI
    std::unique_ptr<lc_gui::Renderer> loadingGUI;
    std::unique_ptr<lc_gui::Element> loadingFullscreenContainer, loadingContainer, loadingProgressContainer, loadingProgress;
    std::unique_ptr<lc_gui::TextElement> loadingText, savingText, pitchChangeText, bounceText;
    lc_gui::Style::SIZE *loadingProgressWidth{nullptr};

    bool saveProject{false}, pitchChange{false}, bounceProject{false};
    size_t pitchChangeIndex;

    lc_gui::KeyHandler keyHandler;
    float prevVolumeValue, prevMetronomeValue;

    enum FileExplorerType
    {
        FILE_EXPLORER_ADD_FILE,
        FILE_EXPLORER_BOUNCE_PROJECT
    };
    FileExplorerType explorerType{FILE_EXPLORER_ADD_FILE};

    void createWaveformCallbacks();
    void createWaveformDataCallbacks();
    void createKeyframeEditorCallbacks();
    void createFileManagerCallback();
    void createCallbacks();

    void createWaveformLmbTypeGUI();
    void createMainControlsGUI();
    void createKeyframeEditorGUI();
    void createLoadingGUI();

    void updateWaveformData();
    void waveformScrollbarResize();
    void updateFileManager(bool reset);
    void updateFiles();

    void frameProgress(int &prevProgress, int &currentProgress, double &prevProgressTime, bool &waveformInit, const double &currentTime);
    void createKeyHandler();
};