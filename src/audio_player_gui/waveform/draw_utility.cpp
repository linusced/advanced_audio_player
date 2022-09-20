#include "audio_player_gui/waveform.hpp"

void audio_player_gui::Waveform::drawPixel(int x, int y, int colorIndex)
{
    if (x < 0 || x >= textureData.width || y < 0 || y >= textureData.height)
        return;

    size_t i = ((textureData.height - y - 1) * textureData.width + x) * 3UL;

    const glm::u8vec3 *color = &waveformColor;
    if (colorIndex == 1)
        color = &beatColor;
    else if (colorIndex == 2)
        color = &amplitudeColor;
    else if (colorIndex == 3)
        color = &lowPassColor;
    else if (colorIndex == 4)
        color = &highPassColor;
    else if (colorIndex == 5)
        color = &automationKeyframePosColor;
    else if (colorIndex == 6)
        color = &automationKeyframePosActiveColor;

    for (int c = 0; c < 3; c++)
        textureData.bytes[i + c] = color->operator[](c);
}

void audio_player_gui::Waveform::drawLine(glm::ivec2 begin, glm::ivec2 end, int colorIndex)
{
    if ((begin.x < 0 && end.x < 0) || (begin.x >= textureData.width && end.x >= textureData.width) || (begin.y < 0 && end.y < 0) || (begin.y > textureData.height && end.y > textureData.height))
        return;

    if (begin.x == textureData.width)
        begin.x = textureData.width - 1;
    if (end.x == textureData.width)
        end.x = textureData.width - 1;

    if (begin.y == textureData.height)
        begin.y = textureData.height - 1;
    if (end.y == textureData.height)
        end.y = textureData.height - 1;

    bool steep = abs(end.y - begin.y) > abs(end.x - begin.x);
    if (steep)
    {
        std::swap(begin.x, begin.y);
        std::swap(end.x, end.y);
    }

    if (begin.x > end.x)
    {
        std::swap(begin.x, end.x);
        std::swap(begin.y, end.y);
    }

    const float dx = end.x - begin.x;
    const float dy = fabs(end.y - begin.y);

    float error = dx / 2.0f;
    const int ystep = (begin.y < end.y) ? 1 : -1;
    int y = (int)begin.y;

    const int maxX = (int)end.x;

    for (int x = (int)begin.x; x <= maxX; x++)
    {
        if (steep)
        {
            drawPixel(y, x, colorIndex);

            if (y > 0)
                drawPixel(y - 1, x, colorIndex);
            if (y + 1 < textureData.width)
                drawPixel(y + 1, x, colorIndex);
        }
        else
        {
            drawPixel(x, y, colorIndex);

            if (y > 0)
                drawPixel(x, y - 1, colorIndex);
            if (y + 1 < textureData.height)
                drawPixel(x, y + 1, colorIndex);
        }

        error -= dy;
        if (error < 0)
        {
            y += ystep;
            error += dx;
        }
    }
}