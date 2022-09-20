#include "draw_loops/main_loop.hpp"

void MainLoop::createKeyframeEditorGUI()
{
    keyframeEditor = std::make_unique<lc_gui::Element>("keyframe-editor");
    keyframeEditor->isVisible = false;
    guiRenderer->addChild(keyframeEditor.get());

    keyframeEditorPosX = &keyframeEditor->elementStyle.sizeProperties["left"];
    keyframeEditorPosY = &keyframeEditor->elementStyle.sizeProperties["top"];
    keyframeEditorPosX->isSet = keyframeEditorPosY->isSet = true;
    keyframeEditorPosX->unit = keyframeEditorPosY->unit = lc_gui::Style::PIXELS;

    keyframeEditorText = std::make_unique<lc_gui::TextElement>(font, "Keyframe Editor", "keyframe-editor-text", false, false);
    keyframeEditor->addChild(keyframeEditorText.get());

    keyframeEditorAmplitudeLabel = std::make_unique<lc_gui::TextElement>(font, "Amplitude:", "keyframe-editor-label keyframe-editor-amplitude-label", false, false);
    keyframeEditor->addChild(keyframeEditorAmplitudeLabel.get());

    keyframeEditorAmplitudeInput = std::make_unique<lc_gui::TextInputElement>(font, "", "keyframe-editor-input keyframe-editor-amplitude-input", keyframeEditorInputCallback);
    keyframeEditorAmplitudeInput->id = "amplitude";
    keyframeEditor->addChild(keyframeEditorAmplitudeInput.get());

    keyframeEditorAmplitudeSlider = std::make_unique<lc_gui::SliderElement>(0.0f, 1.0f, 1.0f, keyframeEditorSliderCallback, "keyframe-editor-slider keyframe-editor-amplitude-slider", "keyframe-editor-slider-mouse-down");
    keyframeEditorAmplitudeSlider->id = "amplitude";
    keyframeEditorAmplitudeSlider->callbackOnInput = true;
    keyframeEditor->addChild(keyframeEditorAmplitudeSlider.get());

    keyframeEditorLowPassFrequencyLabel = std::make_unique<lc_gui::TextElement>(font, "Low-pass Hz:", "keyframe-editor-label keyframe-editor-low-pass-frequency-label", false, false);
    keyframeEditor->addChild(keyframeEditorLowPassFrequencyLabel.get());

    keyframeEditorLowPassFrequencyInput = std::make_unique<lc_gui::TextInputElement>(font, "", "keyframe-editor-input keyframe-editor-low-pass-frequency-input", keyframeEditorInputCallback);
    keyframeEditorLowPassFrequencyInput->id = "low-pass-frequency";
    keyframeEditor->addChild(keyframeEditorLowPassFrequencyInput.get());

    keyframeEditorLowPassFrequencySlider = std::make_unique<lc_gui::SliderElement>(0.0f, 1.0f, 1.0f, keyframeEditorSliderCallback, "keyframe-editor-slider keyframe-editor-low-pass-frequency-slider", "keyframe-editor-slider-mouse-down");
    keyframeEditorLowPassFrequencySlider->id = "low-pass-frequency";
    keyframeEditorLowPassFrequencySlider->callbackOnInput = true;
    keyframeEditor->addChild(keyframeEditorLowPassFrequencySlider.get());

    keyframeEditorHighPassFrequencyLabel = std::make_unique<lc_gui::TextElement>(font, "High-pass Hz:", "keyframe-editor-label keyframe-editor-high-pass-frequency-label", false, false);
    keyframeEditor->addChild(keyframeEditorHighPassFrequencyLabel.get());

    keyframeEditorHighPassFrequencyInput = std::make_unique<lc_gui::TextInputElement>(font, "", "keyframe-editor-input keyframe-editor-high-pass-frequency-input", keyframeEditorInputCallback);
    keyframeEditorHighPassFrequencyInput->id = "high-pass-frequency";
    keyframeEditor->addChild(keyframeEditorHighPassFrequencyInput.get());

    keyframeEditorHighPassFrequencySlider = std::make_unique<lc_gui::SliderElement>(0.0f, 1.0f, 0.0f, keyframeEditorSliderCallback, "keyframe-editor-slider keyframe-editor-high-pass-frequency-slider", "keyframe-editor-slider-mouse-down");
    keyframeEditorHighPassFrequencySlider->id = "high-pass-frequency";
    keyframeEditorHighPassFrequencySlider->callbackOnInput = true;
    keyframeEditor->addChild(keyframeEditorHighPassFrequencySlider.get());
}