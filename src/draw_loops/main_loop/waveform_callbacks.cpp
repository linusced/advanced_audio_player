#include "draw_loops/main_loop.hpp"

void MainLoop::createWaveformCallbacks()
{
    waveformElement->onLmbTypeChange = [this](audio_player_gui::Waveform::LMB_TYPE type)
    {
        for (int i = 0; i < waveformLmbTypeArr.size(); i++)
        {
            auto &btn = waveformLmbTypeArr[i].btn;
            btn->styleChange = true;
            if (i == type)
                btn->classList.add("waveform-lmb-type-btn-active");
            else
                btn->classList.remove("waveform-lmb-type-btn-active");
        }
    };

    waveformElement->onTimeChange = [this](float time)
    {
        audioPlayer->setCurrentTime(time);
    };

    waveformElement->onWaveformMove = [this](size_t offset, size_t index)
    {
        audioPlayer->setFilePlayOffset(offset, index);
        waveformElement->setOffset(audioPlayer->getFilePlayOffset(index), index);
    };

    waveformElement->onKeyframeSelect = [this](AdvancedAudioPlayer::Keyframe *kf, size_t waveformIndex)
    {
        if (!keyframeEditor->isVisible)
            displayKeyframEditor = 2;
        keyframeEditor->styleChange = true;
        currentKeyframe = kf;

        std::string str = std::to_string(kf->amplitude);
        str = str.substr(0, str.find('.') + 3);
        keyframeEditorAmplitudeInput->setText(str);
        keyframeEditorAmplitudeSlider->setValue(kf->amplitude);

        str = std::to_string(kf->lowPassFrequency);
        str = str.substr(0, str.find('.') + 2);
        keyframeEditorLowPassFrequencyInput->setText(str);
        keyframeEditorLowPassFrequencySlider->setValue(AdvancedAudioPlayer::exponentialFrequencyRange(kf->lowPassFrequency));

        str = std::to_string(kf->highPassFrequency);
        str = str.substr(0, str.find('.') + 2);
        keyframeEditorHighPassFrequencyInput->setText(str);
        keyframeEditorHighPassFrequencySlider->setValue(AdvancedAudioPlayer::exponentialFrequencyRange(kf->highPassFrequency));

        double d_cursorX, d_cursorY;
        glfwGetCursorPos(*window, &d_cursorX, &d_cursorY);
        int cursorX = d_cursorX, cursorY = d_cursorY;

        auto l = keyframeEditor->getPreviousLayout();
        int waveformHeight = waveformDataArr[0].container->getPreviousLayout().height;

        keyframeEditorPosX->value = std::clamp(cursorX - l.width / 2, 0, window->width() - l.width);

        keyframeEditorPosY->value = (waveformIndex + 1) * waveformHeight;
        if (keyframeEditorPosY->value > window->height() - l.height)
            keyframeEditorPosY->value = std::clamp((int)waveformIndex * waveformHeight - l.height, 0, window->height());
    };

    waveformElement->onKeyframeClear = [this]()
    {
        keyframeEditor->isVisible = false;
        currentKeyframe = nullptr;
    };
}