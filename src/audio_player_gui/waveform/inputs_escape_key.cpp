#include "audio_player_gui/waveform.hpp"

void audio_player_gui::Waveform::inputsEscapeKey(bool &b_drawWaveforms)
{
    if (glfwGetKey(*window, GLFW_KEY_ESCAPE))
    {
        if (!escapeDown)
        {
            if (waveformDragIndex != -1)
            {
                waveforms[waveformDragIndex].offset = waveformDragPreGlobalOffset;
                waveformDragIndex = -1;
                b_drawWaveforms = true;
            }

            if (selectedKeyframeIndex != -1 && lmbType == LMB_MOVE_KEYFRAME)
            {
                auto &a = waveforms[selectedKeyframeWaveIndex].automation;
                int offset = selectedKeyframeIndex < selectedKeyframeInitIndex ? 1 : -1;

                for (; selectedKeyframeIndex != selectedKeyframeInitIndex; selectedKeyframeIndex += offset)
                    std::swap(a->at(selectedKeyframeIndex), a->at(selectedKeyframeIndex + offset));

                a->at(selectedKeyframeIndex).time = selectedKeyframePreMoveTime;

                selectedKeyframeWaveIndex = selectedKeyframeIndex = -1;
                b_drawWaveforms = true;
            }

            if (createKeyframeWaveIndex != -1)
            {
                createKeyframeWaveIndex = -1;
                b_drawWaveforms = true;
            }

            escapeDown = true;
        }
    }
    else
        escapeDown = false;
}