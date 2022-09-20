#include "draw_loops/main_loop.hpp"

void MainLoop::createWaveformLmbTypeGUI()
{
    waveformLmbTypeArr.push_back(WaveformLmbType());
    waveformLmbTypeArr.back().btn = std::make_unique<lc_gui::ButtonElement>(waveformLmbTypeCallback, "waveform-lmb-type-btn waveform-lmb-type-btn-active", "btn-click");
    waveformLmbTypeArr.back().btn->id = "0";
    waveformLmbTypeContainer->addChild(waveformLmbTypeArr.back().btn.get());

    auto *posY = &waveformLmbTypeArr.back().btn->elementStyle.sizeProperties["top"];
    posY->isSet = true;
    posY->unit = lc_gui::Style::PIXELS;
    posY->value = 50.0f;

    waveformLmbTypeArr.back().text = std::make_unique<lc_gui::TextElement>(font, "[1] Set play time", "btn-text btn-text-small", true, false);
    waveformLmbTypeArr.back().btn->addChild(waveformLmbTypeArr.back().text.get());

    waveformLmbTypeArr.push_back(WaveformLmbType());
    waveformLmbTypeArr.back().btn = std::make_unique<lc_gui::ButtonElement>(waveformLmbTypeCallback, "waveform-lmb-type-btn", "btn-click");
    waveformLmbTypeArr.back().btn->id = "1";
    waveformLmbTypeContainer->addChild(waveformLmbTypeArr.back().btn.get());

    posY = &waveformLmbTypeArr.back().btn->elementStyle.sizeProperties["top"];
    posY->isSet = true;
    posY->unit = lc_gui::Style::PIXELS;
    posY->value = 100.0f;

    waveformLmbTypeArr.back().text = std::make_unique<lc_gui::TextElement>(font, "[2] Move waveform", "btn-text btn-text-small", true, false);
    waveformLmbTypeArr.back().btn->addChild(waveformLmbTypeArr.back().text.get());

    waveformLmbTypeArr.push_back(WaveformLmbType());
    waveformLmbTypeArr.back().btn = std::make_unique<lc_gui::ButtonElement>(waveformLmbTypeCallback, "waveform-lmb-type-btn", "btn-click");
    waveformLmbTypeArr.back().btn->id = "2";
    waveformLmbTypeContainer->addChild(waveformLmbTypeArr.back().btn.get());

    posY = &waveformLmbTypeArr.back().btn->elementStyle.sizeProperties["top"];
    posY->isSet = true;
    posY->unit = lc_gui::Style::PIXELS;
    posY->value = 150.0f;

    waveformLmbTypeArr.back().text = std::make_unique<lc_gui::TextElement>(font, "[3] Select keyframe", "btn-text btn-text-small", true, false);
    waveformLmbTypeArr.back().btn->addChild(waveformLmbTypeArr.back().text.get());

    waveformLmbTypeArr.push_back(WaveformLmbType());
    waveformLmbTypeArr.back().btn = std::make_unique<lc_gui::ButtonElement>(waveformLmbTypeCallback, "waveform-lmb-type-btn", "btn-click");
    waveformLmbTypeArr.back().btn->id = "3";
    waveformLmbTypeContainer->addChild(waveformLmbTypeArr.back().btn.get());

    posY = &waveformLmbTypeArr.back().btn->elementStyle.sizeProperties["top"];
    posY->isSet = true;
    posY->unit = lc_gui::Style::PIXELS;
    posY->value = 200.0f;

    waveformLmbTypeArr.back().text = std::make_unique<lc_gui::TextElement>(font, "[4] Move keyframe", "btn-text btn-text-small", true, false);
    waveformLmbTypeArr.back().btn->addChild(waveformLmbTypeArr.back().text.get());

    waveformLmbTypeArr.push_back(WaveformLmbType());
    waveformLmbTypeArr.back().btn = std::make_unique<lc_gui::ButtonElement>(waveformLmbTypeCallback, "waveform-lmb-type-btn", "btn-click");
    waveformLmbTypeArr.back().btn->id = "4";
    waveformLmbTypeContainer->addChild(waveformLmbTypeArr.back().btn.get());

    posY = &waveformLmbTypeArr.back().btn->elementStyle.sizeProperties["top"];
    posY->isSet = true;
    posY->unit = lc_gui::Style::PIXELS;
    posY->value = 250.0f;

    waveformLmbTypeArr.back().text = std::make_unique<lc_gui::TextElement>(font, "[5] Create keyframe", "btn-text btn-text-small", true, false);
    waveformLmbTypeArr.back().btn->addChild(waveformLmbTypeArr.back().text.get());
}