#include "audio_player_gui/waveform.hpp"

void audio_player_gui::Waveform::inputsSetCursor(const int &cursorWaveIndex, int &cursorKeyframeIndex, const long &cursorWaveRelativePos)
{
    if (lmbType == LMB_TIME_CHANGE)
    {
        window->setCursor(GLFW_HRESIZE_CURSOR);
    }
    else if (lmbType == LMB_MOVE_WAVEFORM)
    {
        if (waveformDragIndex != -1 || (cursorWaveIndex != 0 && cursorWaveRelativePos >= 0 && cursorWaveRelativePos < waveforms[cursorWaveIndex].bufferSizePerChannels))
            window->setCursor(GLFW_HAND_CURSOR);
    }
    else if (lmbType == LMB_SELECT_KEYFRAME || lmbType == LMB_MOVE_KEYFRAME)
    {
        if (selectedKeyframeWaveIndex == -1 || lmbType == LMB_SELECT_KEYFRAME)
        {
            long maxOffset = zoom / 200L;

            for (int i = 0; i < waveforms[cursorWaveIndex].automation->size(); i++)
                if (std::abs((long)waveforms[cursorWaveIndex].automation->at(i).time - cursorWaveRelativePos) < maxOffset)
                {
                    cursorKeyframeIndex = i;
                    break;
                }

            if (cursorKeyframeIndex != -1)
                window->setCursor(GLFW_HAND_CURSOR);
        }
        else
            window->setCursor(GLFW_HAND_CURSOR);
    }
    else if (lmbType == LMB_CREATE_KEYFRAME)
        window->setCursor(GLFW_HAND_CURSOR);
}