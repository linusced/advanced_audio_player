#include "draw_loops/main_loop.hpp"

void MainLoop::createKeyframeEditorCallbacks()
{
    keyframeEditorInputCallback = [this](lc_gui::TextInputElement *input, std::string &str)
    {
        bool isAmplitude = input->id == "amplitude";
        try
        {
            size_t endPos;
            float value = std::stof(str, &endPos);
            if (endPos != str.size())
                throw "";

            value = std::clamp(value, isAmplitude ? 0.0f : AdvancedAudioPlayer::MIN_FREQUENCY, isAmplitude ? 1.0f : AdvancedAudioPlayer::MAX_FREQUENCY);
            float valueRange = AdvancedAudioPlayer::exponentialFrequencyRange(value);

            str = std::to_string(value);
            str = str.substr(0, str.find('.') + (isAmplitude ? 3 : 2));

            if (isAmplitude)
            {
                keyframeEditorAmplitudeSlider->setValue(value);
                currentKeyframe->amplitude = value;
            }
            else if (input->id == "low-pass-frequency")
            {
                keyframeEditorLowPassFrequencySlider->setValue(valueRange);
                currentKeyframe->lowPassFrequency = value;
            }
            else
            {
                keyframeEditorHighPassFrequencySlider->setValue(valueRange);
                currentKeyframe->highPassFrequency = value;
            }

            waveformElement->drawWaveforms();
        }
        catch (...)
        {
            if (isAmplitude)
                str = std::to_string(currentKeyframe->amplitude);
            else if (input->id == "low-pass-frequency")
                str = std::to_string(currentKeyframe->lowPassFrequency);
            else
                str = std::to_string(currentKeyframe->highPassFrequency);

            str = str.substr(0, str.find('.') + (isAmplitude ? 3 : 2));
        }
    };

    keyframeEditorSliderCallback = [this](lc_gui::SliderElement *slider, float &valueRange)
    {
        bool isAmplitude = slider->id == "amplitude", lmbDown = window->isLmbDown();

        float value = isAmplitude ? valueRange : AdvancedAudioPlayer::exponentialFrequencyRangeInverse(valueRange);
        std::string str = std::to_string(value);
        str = str.substr(0, str.find('.') + (isAmplitude ? 3 : 2));

        if (isAmplitude)
        {
            keyframeEditorAmplitudeInput->setText(str);
            if (!lmbDown)
                currentKeyframe->amplitude = value;
        }
        else if (slider->id == "low-pass-frequency")
        {
            keyframeEditorLowPassFrequencyInput->setText(str);
            if (!lmbDown)
                currentKeyframe->lowPassFrequency = value;
        }
        else
        {
            keyframeEditorHighPassFrequencyInput->setText(str);
            if (!lmbDown)
                currentKeyframe->highPassFrequency = value;
        }

        if (!lmbDown)
            waveformElement->drawWaveforms();
    };
}