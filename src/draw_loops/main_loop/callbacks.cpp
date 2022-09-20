#include "draw_loops/main_loop.hpp"

void MainLoop::createCallbacks()
{
    createWaveformCallbacks();
    createWaveformDataCallbacks();
    createKeyframeEditorCallbacks();
    createFileManagerCallback();

    waveformLmbTypeCallback = [this](lc_gui::ButtonElement *btn)
    {
        waveformElement->setLmbType((audio_player_gui::Waveform::LMB_TYPE)std::stoi(btn->id));
    };

    waveformScrollCallback = [this](int scroll)
    {
        waveformY->value = -scroll;
        waveformContainer->styleChange = true;
    };

    mainControlsBtnCallback = [this](lc_gui::ButtonElement *btn)
    {
        if (btn->id == "toggle-playback")
        {
            if (audioPlayer->isPlaying())
            {
                audioPlayer->pause();
                togglePlaybackText->setText("Play");
            }
            else
            {
                audioPlayer->play();
                togglePlaybackText->setText("Pause");
            }
        }
        else if (btn->id == "save-project")
        {
            saveProject = true;
        }
        else if (btn->id == "bounce-project")
        {
            bounceProject = true;
        }
        else if (btn->id == "toggle-file-manager")
        {
            if (fileManagerContainer->isVisible)
            {
                fileManagerContainer->isVisible = false;
                toggleFileManagerText->setText("Show file manager");
            }
            else
            {
                fileManagerContainer->isVisible = true;
                fileManagerResetBtn->isVisible = fileManagerSaveBtn->isVisible = false;
                toggleFileManagerText->setText("Hide file manager");
            }
        }
    };

    mainControlsSliderCallback = [this](lc_gui::SliderElement *slider, float &value)
    {
        if (value < 0.05f)
            value = 0.0f;
        else if (value > 0.95f)
            value = 1.0f;

        if (slider->id == "volume")
            audioPlayer->setVolume(value);
        else if (slider->id == "metronome")
            audioPlayer->setMetronomeVolume(value);
    };
}