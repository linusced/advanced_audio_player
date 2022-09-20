#include "audio_player_gui/waveform.hpp"

void audio_player_gui::Waveform::inputsLmbType(const glm::ivec2 &relativeCursorPosition, const int &cursorWaveIndex, const long &cursorWaveRelativePos, bool &b_drawWaveforms)
{
    if (lmbType == LMB_MOVE_WAVEFORM && waveformDragIndex != -1)
    {
        long offset = (relativeCursorPosition.x / (double)prevLayout.width) * zoom + panning - waveformDragOffset;
        if (offset < 0)
            offset = 0;

        if (glfwGetKey(*window, GLFW_KEY_LEFT_SHIFT) || glfwGetKey(*window, GLFW_KEY_RIGHT_SHIFT))
        {
            double prevFileBeatInterval = waveforms[waveformDragIndex - 1].beatInterval,
                   prevFileBeatOffset = waveforms[waveformDragIndex - 1].beatOffset + waveforms[waveformDragIndex - 1].offset / D_BUFFER_SAMPLE_RATE,
                   fileBeatOffset = waveforms[waveformDragIndex].beatOffset,
                   d_offset = offset / D_BUFFER_SAMPLE_RATE;

            d_offset = std::round((d_offset - prevFileBeatOffset + fileBeatOffset) / prevFileBeatInterval) * prevFileBeatInterval + prevFileBeatOffset - fileBeatOffset;

            if (d_offset < 0.0)
                d_offset = 0.0;

            offset = d_offset * BUFFER_SAMPLE_RATE;
        }

        waveforms[waveformDragIndex].offset = offset;
        b_drawWaveforms = true;
    }
    else if (lmbType == LMB_SELECT_KEYFRAME && selectedKeyframeIndex != -1 && (glfwGetKey(*window, GLFW_KEY_X) || glfwGetKey(*window, GLFW_KEY_BACKSPACE) || glfwGetKey(*window, GLFW_KEY_DELETE)))
    {
        waveforms[selectedKeyframeWaveIndex].automation->erase(waveforms[selectedKeyframeWaveIndex].automation->begin() + selectedKeyframeIndex);
        selectedKeyframeWaveIndex = selectedKeyframeIndex = -1;
        onKeyframeClear();
        b_drawWaveforms = true;
    }
    else if (lmbType == LMB_MOVE_KEYFRAME && selectedKeyframeIndex != -1)
    {
        auto &a = waveforms[selectedKeyframeWaveIndex].automation;
        size_t newTime = std::clamp(cursorWaveRelativePos, 0L, (long)waveforms[selectedKeyframeWaveIndex].bufferSizePerChannels);

        if (glfwGetKey(*window, GLFW_KEY_LEFT_SHIFT) || glfwGetKey(*window, GLFW_KEY_RIGHT_SHIFT))
        {
            double d_newTime = newTime / D_BUFFER_SAMPLE_RATE;
            d_newTime = std::round((d_newTime - waveforms[selectedKeyframeWaveIndex].beatOffset) / waveforms[selectedKeyframeWaveIndex].beatInterval) * waveforms[selectedKeyframeWaveIndex].beatInterval + waveforms[selectedKeyframeWaveIndex].beatOffset;

            if (d_newTime < waveforms[selectedKeyframeWaveIndex].beatOffset)
                d_newTime = waveforms[selectedKeyframeWaveIndex].beatOffset;

            newTime = d_newTime * BUFFER_SAMPLE_RATE;
        }

        if (selectedKeyframeIndex > 0 && newTime < a->at(selectedKeyframeIndex - 1).time)
        {
            std::swap(a->at(selectedKeyframeIndex), a->at(selectedKeyframeIndex - 1));
            selectedKeyframeIndex--;
        }
        else if (selectedKeyframeIndex < a->size() - 1 && newTime > a->at(selectedKeyframeIndex + 1).time)
        {
            std::swap(a->at(selectedKeyframeIndex), a->at(selectedKeyframeIndex + 1));
            selectedKeyframeIndex++;
        }

        a->at(selectedKeyframeIndex).time = newTime;
        b_drawWaveforms = true;
    }
    else if (lmbType == LMB_CREATE_KEYFRAME)
    {
        createKeyframeWaveIndex = cursorWaveIndex;
        createKeyframeTime = std::clamp(cursorWaveRelativePos, 0L, (long)waveforms[createKeyframeWaveIndex].bufferSizePerChannels);

        if (glfwGetKey(*window, GLFW_KEY_LEFT_SHIFT) || glfwGetKey(*window, GLFW_KEY_RIGHT_SHIFT))
        {
            double d_newTime = createKeyframeTime / D_BUFFER_SAMPLE_RATE;
            d_newTime = std::round((d_newTime - waveforms[createKeyframeWaveIndex].beatOffset) / waveforms[createKeyframeWaveIndex].beatInterval) * waveforms[createKeyframeWaveIndex].beatInterval + waveforms[createKeyframeWaveIndex].beatOffset;

            if (d_newTime < waveforms[createKeyframeWaveIndex].beatOffset)
                d_newTime = waveforms[createKeyframeWaveIndex].beatOffset;

            createKeyframeTime = d_newTime * BUFFER_SAMPLE_RATE;
        }

        b_drawWaveforms = true;
    }
}