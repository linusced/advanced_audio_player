#include "audio_player_gui/waveform.hpp"

void audio_player_gui::Waveform::drawWaveforms()
{
    if (maxTime == 0.0)
        return;

    for (auto &i : textureData.bytes)
        i = 0;

    const size_t numWaveforms = waveforms.size(),
                 waveformHeight = textureData.height / numWaveforms;

    long minY, maxY, x, y, w;
    const float zoomedMaxTime = zoom / (double)totalSize * maxTime;
    float zoomedOffsetTime, loopEnd, t;

    for (w = 0; w < numWaveforms; w++)
        if (waveforms[w].beatInterval != 0.0)
        {
            zoomedOffsetTime = (panning - (long)waveforms[w].offset) / D_BUFFER_SAMPLE_RATE;
            loopEnd = zoomedMaxTime + zoomedOffsetTime;

            if (1.0 / waveforms[w].beatInterval * zoomedMaxTime < 200.0)
                for (t = waveforms[w].beatOffset; t < loopEnd; t += waveforms[w].beatInterval)
                {
                    x = (t - zoomedOffsetTime) / zoomedMaxTime * textureData.width;

                    minY = w * waveformHeight;
                    maxY = (w + 1) * waveformHeight;
                    for (y = minY; y < maxY; y++)
                    {
                        drawPixel(x - 1, y, 1);
                        drawPixel(x, y, 1);
                        drawPixel(x + 1, y, 1);
                    }

                    if (t > waveforms[w].maxTime)
                        break;
                }
        }

    const long step = std::round(zoom / (double)textureData.width),
               bufferSkip = std::clamp((long)std::pow(zoom / 200000.0, 2), 1L, zoom / 100000);

    long i, bufferIndex;
    float min, max, value;

    for (x = 0; x < textureData.width; x++)
        for (w = 0; w < numWaveforms; w++)
        {
            min = 1.0f;
            max = -1.0f;
            for (i = 0; i < step; i += bufferSkip)
            {
                bufferIndex = x * step + i + panning - waveforms[w].offset;
                if (bufferIndex < 0L || bufferIndex >= waveforms[w].bufferSizePerChannels)
                    break;

                value = waveforms[w].data->at(bufferIndex * BUFFER_NUM_CHANNELS);
                if (value < min)
                    min = value;
                if (value > max)
                    max = value;
            }

            minY = (min / 2.0f + 0.5f) * waveformHeight + (w * waveformHeight);
            maxY = (max / 2.0f + 0.5f) * waveformHeight + (w * waveformHeight);

            if (maxY - minY == 0)
                drawPixel(x, minY, 0);
            else
                for (y = minY; y < maxY; y++)
                    drawPixel(x, y, 0);
        }

    glm::ivec2 prevAutomationCoords, automationCoords;
    long relativeTime;
    int colorIndex;

    for (w = 0; w < numWaveforms; w++)
        if (!waveforms[w].automation->empty())
        {
            if (waveforms[w].automation->size() > 1)
                for (i = 0; i < 3; i++)
                {
                    if (i == 0)
                        value = waveforms[w].automation->front().amplitude;
                    else if (i == 1)
                        value = AdvancedAudioPlayer::exponentialFrequencyRange(waveforms[w].automation->front().lowPassFrequency);
                    else
                        value = AdvancedAudioPlayer::exponentialFrequencyRange(waveforms[w].automation->front().highPassFrequency);

                    relativeTime = waveforms[w].offset;
                    prevAutomationCoords.x = ((relativeTime - panning) / (float)zoom) * textureData.width;

                    relativeTime = waveforms[w].automation->front().time + waveforms[w].offset;
                    automationCoords.x = ((relativeTime - panning) / (float)zoom) * textureData.width;
                    automationCoords.y = (1.0f - value) * waveformHeight + (w * waveformHeight);

                    if (automationCoords.y == textureData.height)
                        automationCoords.y -= 2;

                    prevAutomationCoords.x = std::clamp(prevAutomationCoords.x, 0, textureData.width);
                    automationCoords.x = std::clamp(automationCoords.x, 0, textureData.width);

                    for (x = prevAutomationCoords.x; x < automationCoords.x; x++)
                        for (y = automationCoords.y - 1; y <= automationCoords.y + 1; y++)
                            drawPixel(x, y, i + 2);

                    if (i == 0)
                        value = waveforms[w].automation->back().amplitude;
                    else if (i == 1)
                        value = AdvancedAudioPlayer::exponentialFrequencyRange(waveforms[w].automation->back().lowPassFrequency);
                    else
                        value = AdvancedAudioPlayer::exponentialFrequencyRange(waveforms[w].automation->back().highPassFrequency);

                    relativeTime = waveforms[w].automation->back().time + waveforms[w].offset;
                    prevAutomationCoords.x = ((relativeTime - panning) / (float)zoom) * textureData.width;

                    relativeTime = waveforms[w].bufferSizePerChannels + waveforms[w].offset;
                    automationCoords.x = ((relativeTime - panning) / (float)zoom) * textureData.width;
                    automationCoords.y = (1.0f - value) * waveformHeight + (w * waveformHeight);

                    if (automationCoords.y == textureData.height)
                        automationCoords.y -= 2;

                    prevAutomationCoords.x = std::clamp(prevAutomationCoords.x, 0, textureData.width);
                    automationCoords.x = std::clamp(automationCoords.x, 0, textureData.width);

                    for (x = prevAutomationCoords.x; x < automationCoords.x; x++)
                        for (y = automationCoords.y - 1; y <= automationCoords.y + 1; y++)
                            drawPixel(x, y, i + 2);

                    prevAutomationCoords = glm::ivec2(-1);

                    for (auto &a : *waveforms[w].automation)
                    {
                        if (i == 0)
                            value = a.amplitude;
                        else if (i == 1)
                            value = AdvancedAudioPlayer::exponentialFrequencyRange(a.lowPassFrequency);
                        else
                            value = AdvancedAudioPlayer::exponentialFrequencyRange(a.highPassFrequency);

                        relativeTime = a.time + waveforms[w].offset;
                        automationCoords.x = ((relativeTime - panning) / (float)zoom) * textureData.width;
                        automationCoords.y = (1.0f - value) * waveformHeight + (w * waveformHeight);

                        if (prevAutomationCoords.x != -1)
                            drawLine(prevAutomationCoords, automationCoords, i + 2);

                        prevAutomationCoords = automationCoords;
                    }
                }

            for (i = 0; i < waveforms[w].automation->size(); i++)
            {
                relativeTime = waveforms[w].automation->at(i).time + waveforms[w].offset;
                x = ((relativeTime - panning) / (float)zoom) * textureData.width;

                minY = w * waveformHeight;
                maxY = (w + 1) * waveformHeight;
                for (y = minY; y < maxY; y++)
                {
                    colorIndex = 5;
                    if (w == selectedKeyframeWaveIndex && i == selectedKeyframeIndex)
                        colorIndex = 6;

                    drawPixel(x - 1, y, colorIndex);
                    drawPixel(x, y, colorIndex);
                    drawPixel(x + 1, y, colorIndex);
                }
            }
        }

    if (createKeyframeWaveIndex != -1)
    {
        w = createKeyframeWaveIndex;
        relativeTime = createKeyframeTime + waveforms[w].offset;
        x = ((relativeTime - panning) / (float)zoom) * textureData.width;

        colorIndex = 6;
        minY = w * waveformHeight;
        maxY = (w + 1) * waveformHeight;
        for (y = minY; y < maxY; y++)
        {
            drawPixel(x - 1, y, colorIndex);
            drawPixel(x, y, colorIndex);
            drawPixel(x + 1, y, colorIndex);
        }
    }

    texture->update(textureData.bytes, textureData.width, textureData.height, textureData.format);

    styleChange = true;
    float playHeadTime = prevPlayHeadTime;
    prevPlayHeadTime = -1.0;
    setPlayHead(playHeadTime);
}