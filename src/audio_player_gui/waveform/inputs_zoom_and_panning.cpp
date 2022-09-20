#include "audio_player_gui/waveform.hpp"

void audio_player_gui::Waveform::inputsZoomAndPanning(const glm::ivec2 &relativeCursorPosition, const float &cursorClampedX, const float &cursorTime, bool &b_drawWaveforms)
{
    double scrollY = window->getScrollY(), scrollX = window->getScrollX();
    if (scrollY != 0.0)
    {
        zoom = std::clamp(zoom + (long)(scrollY * -10.0 * BUFFER_SAMPLE_RATE), (long)BUFFER_SAMPLE_RATE * 2L, totalSize);

        float newCursorTime = cursorClampedX * (zoom / (double)totalSize * maxTime) + panning / D_BUFFER_SAMPLE_RATE;
        panning = std::clamp(panning + (long)((cursorTime - newCursorTime) * BUFFER_SAMPLE_RATE), 0L, totalSize - zoom);

        b_drawWaveforms = true;
    }

    if (scrollX != 0.0)
    {
        panning = std::clamp(panning + (long)((scrollX * -0.75) * BUFFER_SAMPLE_RATE), 0L, totalSize - zoom);
        b_drawWaveforms = true;
    }
    else if (glfwGetMouseButton(*window, GLFW_MOUSE_BUTTON_MIDDLE))
    {
        if (mmbDown)
        {
            styleChange = true;

            int cursorOffset = mmbDownXOffset - relativeCursorPosition.x;
            if (relativeCursorPosition.x <= 0)
                cursorOffset = 40;
            else if (relativeCursorPosition.x >= prevLayout.width - 1)
                cursorOffset = -40;

            size_t offset = std::clamp(panning + cursorOffset * (zoom / prevLayout.width), 0L, totalSize - zoom);
            if (offset != panning)
            {
                panning = offset;
                b_drawWaveforms = true;
            }
            mmbDownXOffset = relativeCursorPosition.x;
        }
        else
        {
            mmbDown = true;
            mmbDownXOffset = relativeCursorPosition.x;
        }
    }
    else if (mmbDown)
        mmbDown = false;
}