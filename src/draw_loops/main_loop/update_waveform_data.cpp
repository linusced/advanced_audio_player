#include "draw_loops/main_loop.hpp"

void MainLoop::updateWaveformData()
{
    updateWaveformDataNextFrame = false;

    waveformContainer->children.clear();
    waveformContainer->addChild(waveformElement.get());

    const size_t numFiles = audioPlayer->getNumFiles();
    waveformElement->setNumWaveforms(numFiles);

    for (auto &data : waveformDataArr)
    {
        data.bpmInput->stopThread();
        data.beatOffsetInput->stopThread();
        data.pitchChangeBpmInput->stopThread();
    }

    waveformDataArr.clear();
    waveformDataArr.resize(numFiles);

    auto *height = &waveformContainer->elementStyle.sizeProperties["height"];
    height->isSet = true;
    height->unit = lc_gui::Style::PIXELS;
    height->value = WAVEFORM_HEIGHT * numFiles;

    for (size_t i = 0; i < numFiles; i++)
    {
        waveformElement->setData(&audioPlayer->getFile(i)->getFullBuffer(), audioPlayer->getFileAutomation(i), i);
        waveformElement->setBeats(audioPlayer->getFileBeats(i)->getBeatInterval(), audioPlayer->getFileBeats(i)->getBeatOffset(), i);
        waveformElement->setOffset(audioPlayer->getFilePlayOffset(i), i);

        auto &w = waveformDataArr[i];
        w.container = std::make_unique<lc_gui::Element>("waveform-data");
        auto *y = &w.container->elementStyle.sizeProperties.at("top");
        height = &w.container->elementStyle.sizeProperties.at("height");

        y->isSet = height->isSet = true;
        y->unit = height->unit = lc_gui::Style::PERCENTAGE;
        height->value = 100.0f / numFiles;
        y->value = height->value * i;

        waveformContainer->addChild(w.container.get());

        std::string indexStr = std::to_string(i),
                    nameStr = audioPlayer->getFileName(i);
        nameStr[0] = toupper(nameStr[0]);
        for (size_t i = 1; i < nameStr.size() - 1; i++)
            if (nameStr[i] == ' ' || nameStr[i] == '_' || nameStr[i] == '-')
            {
                nameStr[i] = ' ';
                nameStr[i + 1] = toupper(nameStr[i + 1]);
                i++;
            }

        w.name = std::make_unique<lc_gui::TextElement>(font, nameStr, "waveform-data-name", false, false);
        w.container->addChild(w.name.get());

        w.muteBtn = std::make_unique<lc_gui::ButtonElement>(waveformDataBtnCallback, "waveform-data-mute", "waveform-data-mute-click");
        w.muteBtn->id = "mute-" + indexStr;
        w.container->addChild(w.muteBtn.get());

        w.muteBtnText = std::make_unique<lc_gui::TextElement>(font, "Mute", "btn-text waveform-data-mute-text", false, false);
        w.muteBtn->addChild(w.muteBtnText.get());

        w.bpmLabel = std::make_unique<lc_gui::TextElement>(font, "BPM:", "waveform-data-element waveform-data-label waveform-data-bpm-label", false, false);
        w.container->addChild(w.bpmLabel.get());

        w.bpmInput = std::make_unique<lc_gui::TextInputElement>(font, std::to_string(audioPlayer->getFileBeats(i)->getBPM()), "waveform-data-element waveform-data-text-input waveform-data-bpm-input", waveformDataInputCallback);
        w.bpmInput->id = "bpm-input-" + indexStr;
        w.bpmInput->onlyCallbackOnEnterPress = w.bpmInput->resetTextOnEscape = true;
        w.container->addChild(w.bpmInput.get());

        w.beatOffsetLabel = std::make_unique<lc_gui::TextElement>(font, "Beat offset:", "waveform-data-element waveform-data-label waveform-data-beat-offset-label", false, false);
        w.container->addChild(w.beatOffsetLabel.get());

        w.beatOffsetInput = std::make_unique<lc_gui::TextInputElement>(font, std::to_string(audioPlayer->getFileBeats(i)->getBeatOffset()), "waveform-data-element waveform-data-text-input waveform-data-beat-offset-input", waveformDataInputCallback);
        w.beatOffsetInput->id = "beat-offset-input-" + indexStr;
        w.beatOffsetInput->onlyCallbackOnEnterPress = w.beatOffsetInput->resetTextOnEscape = true;
        w.container->addChild(w.beatOffsetInput.get());

        w.pitchChangeBpmLabel = std::make_unique<lc_gui::TextElement>(font, "Pitch BPM:", "waveform-data-element waveform-data-label waveform-data-pitch-change-bpm-label", false, false);
        w.container->addChild(w.pitchChangeBpmLabel.get());

        w.pitchChangeBpmInput = std::make_unique<lc_gui::TextInputElement>(font, std::to_string(audioPlayer->getFileBeats(i)->getBPM()), "waveform-data-element waveform-data-text-input waveform-data-pitch-change-bpm-input", waveformDataInputCallback);
        w.pitchChangeBpmInput->id = "pitch-change-bpm-input-" + indexStr;
        w.pitchChangeBpmInput->onlyCallbackOnEnterPress = w.pitchChangeBpmInput->resetTextOnEscape = true;
        w.container->addChild(w.pitchChangeBpmInput.get());

        w.pitchChangeBtn = std::make_unique<lc_gui::ButtonElement>(waveformDataBtnCallback, "waveform-data-pitch-change-btn", "btn-click-light");
        w.pitchChangeBtn->id = "pitch-change-" + indexStr;
        w.container->addChild(w.pitchChangeBtn.get());

        w.pitchChangeText = std::make_unique<lc_gui::TextElement>(font, "Change pitch", "btn-text waveform-data-pitch-change-btn-text", false, false);
        w.pitchChangeBtn->addChild(w.pitchChangeText.get());

        w.pitchBPM = audioPlayer->getFileBeats(i)->getBPM();
    }

    waveformElement->drawWaveforms();
    guiRenderer->resize();
    waveformScrollbarResize();
}