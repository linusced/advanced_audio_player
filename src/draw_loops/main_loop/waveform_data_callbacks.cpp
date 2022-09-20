#include "draw_loops/main_loop.hpp"

void MainLoop::createWaveformDataCallbacks()
{
    waveformDataInputCallback = [this](lc_gui::TextInputElement *input, std::string &str)
    {
        size_t lastDash = input->id.find_last_of('-');
        std::string type = input->id.substr(0, lastDash);
        int index = std::stoi(input->id.substr(lastDash + 1));
        bool isBPM = type.find("bpm-input") != std::string::npos;

        try
        {
            if (isBPM)
            {
                size_t endPos;
                int bpm = std::stoi(str, &endPos);
                if (endPos != str.size() || bpm < 1)
                    throw "";

                if (type == "bpm-input")
                {
                    audioPlayer->getFileBeats(index)->setBPM(bpm);
                    waveformElement->setBeats(audioPlayer->getFileBeats(index)->getBeatInterval(), audioPlayer->getFileBeats(index)->getBeatOffset(), index);
                    waveformElement->drawWaveforms();
                    waveformDataArr[index].pitchChangeBpmInput->setText(std::to_string(bpm));
                }
                else
                    waveformDataArr[index].pitchBPM = bpm;
            }
            else
            {
                size_t endPos;
                double beatOffset = std::stod(str, &endPos);
                if (endPos != str.size() || beatOffset < 0.0)
                    throw "";

                audioPlayer->getFileBeats(index)->setBeatOffset(beatOffset);
                waveformElement->setBeats(audioPlayer->getFileBeats(index)->getBeatInterval(), beatOffset, index);
                waveformElement->drawWaveforms();
            }
        }
        catch (...)
        {
            if (isBPM)
                str = std::to_string(audioPlayer->getFileBeats(index)->getBPM());
            else
                str = std::to_string(audioPlayer->getFileBeats(index)->getBeatOffset());
        }
    };

    waveformDataBtnCallback = [this](lc_gui::ButtonElement *btn)
    {
        size_t lastDash = btn->id.find_last_of('-');
        std::string type = btn->id.substr(0, lastDash);
        int index = std::stoi(btn->id.substr(lastDash + 1));

        if (type == "pitch-change" && waveformDataArr[index].pitchBPM != audioPlayer->getFileBeats(index)->getBPM())
        {
            pitchChange = true;
            pitchChangeIndex = index;
        }
        else if (type == "mute")
        {
            if (audioPlayer->isMuted(index))
            {
                audioPlayer->unmute(index);
                btn->classList.remove("waveform-data-mute-active");
            }
            else
            {
                audioPlayer->mute(index);
                btn->classList.add("waveform-data-mute-active");
            }
            btn->styleChange = true;
        }
    };
}