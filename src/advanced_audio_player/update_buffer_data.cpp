#include "advanced_audio_player/advanced_audio_player.hpp"

void AdvancedAudioPlayer::updateBufferData()
{
    advanced_updateBufferData(false);
}

void AdvancedAudioPlayer::advanced_updateBufferData(bool bounce)
{
    if (!bounce)
    {
        fileLoadingMutex.lock();
        bufferDataMutex.lock();
    }

    bufferOffsetMono = bufferOffset / NUM_CHANNELS;
    currentTime = bufferOffsetMono / D_SAMPLE_RATE;

    bool bufferDataEmpty{true}, lowPass, highPass;
    size_t relativeOffset, automationIndex, i, c;
    float interpolation, amplitude, lowPassFrequency, highPassFrequency,
        a1, out;

    double metronomeInterval{0.0}, metronomeOffset;

    for (auto &f : files)
        if (!f.muted && bufferOffsetMono >= f.playOffset && bufferOffsetMono < f.playOffset + f.duration)
        {
            auto &data = f.file->getData(bufferOffset - (f.playOffset * NUM_CHANNELS), BUFFER_SIZE, nullptr);

            relativeOffset = bufferOffsetMono - f.playOffset;

            if (f.automation.size() > 1)
            {
                if (relativeOffset + BUFFER_SIZE_MONO < f.automation.front().time)
                {
                    if (metronomeInterval == 0.0 && f.automation.front().amplitude != 0.0f)
                    {
                        metronomeInterval = f.beats->getBeatInterval();
                        metronomeOffset = (f.playOffset / D_SAMPLE_RATE) + f.beats->getBeatOffset();
                    }

                    for (auto &sample : data)
                        sample *= f.automation.front().amplitude;

                    if (f.automation.front().lowPassFrequency != MAX_FREQUENCY)
                    {
                        if (f.prevLowPassValues.empty())
                            f.prevLowPassValues.resize(NUM_CHANNELS, 0.0f);

                        lc_audio::lowPassFilter(data, NUM_CHANNELS, SAMPLE_RATE, f.automation.front().lowPassFrequency, f.prevLowPassValues.data());
                    }
                    else
                        f.prevLowPassValues.clear();

                    if (f.automation.front().highPassFrequency != MIN_FREQUENCY)
                    {
                        if (f.prevHighPassValues.empty())
                            f.prevHighPassValues.resize(NUM_CHANNELS, 0.0f);

                        lc_audio::highPassFilter(data, NUM_CHANNELS, SAMPLE_RATE, f.automation.front().highPassFrequency, f.prevHighPassValues.data());
                    }
                    else
                        f.prevHighPassValues.clear();
                }
                else if (relativeOffset >= f.automation.back().time)
                {
                    if (metronomeInterval == 0.0 && f.automation.back().amplitude != 0.0f && relativeOffset < f.duration - f.beats->getBeatInterval() / 2.0)
                    {
                        metronomeInterval = f.beats->getBeatInterval();
                        metronomeOffset = (f.playOffset / D_SAMPLE_RATE) + f.beats->getBeatOffset();
                    }

                    for (auto &sample : data)
                        sample *= f.automation.back().amplitude;

                    if (f.automation.back().lowPassFrequency != MAX_FREQUENCY)
                    {
                        if (f.prevLowPassValues.empty())
                            f.prevLowPassValues.resize(NUM_CHANNELS, 0.0f);

                        lc_audio::lowPassFilter(data, NUM_CHANNELS, SAMPLE_RATE, f.automation.back().lowPassFrequency, f.prevLowPassValues.data());
                    }
                    else
                        f.prevLowPassValues.clear();

                    if (f.automation.back().highPassFrequency != MIN_FREQUENCY)
                    {
                        if (f.prevHighPassValues.empty())
                            f.prevHighPassValues.resize(NUM_CHANNELS, 0.0f);

                        lc_audio::highPassFilter(data, NUM_CHANNELS, SAMPLE_RATE, f.automation.back().highPassFrequency, f.prevHighPassValues.data());
                    }
                    else
                        f.prevHighPassValues.clear();
                }
                else
                {
                    if (metronomeInterval == 0.0 && relativeOffset < f.duration - f.beats->getBeatInterval() / 2.0)
                    {
                        metronomeInterval = f.beats->getBeatInterval();
                        metronomeOffset = (f.playOffset / D_SAMPLE_RATE) + f.beats->getBeatOffset();
                    }

                    for (automationIndex = 0; automationIndex < f.automation.size() - 1; automationIndex++)
                        if (relativeOffset < f.automation[automationIndex + 1].time)
                            break;

                    if (f.automation[automationIndex].lowPassFrequency != MAX_FREQUENCY || f.automation[automationIndex + 1].lowPassFrequency != MAX_FREQUENCY)
                    {
                        if (f.prevLowPassValues.empty())
                            f.prevLowPassValues.resize(NUM_CHANNELS, 0.0f);
                    }
                    else
                        f.prevLowPassValues.clear();

                    lowPass = !f.prevLowPassValues.empty();

                    if (f.automation[automationIndex].highPassFrequency != MIN_FREQUENCY || f.automation[automationIndex + 1].highPassFrequency != MIN_FREQUENCY)
                    {
                        if (f.prevHighPassValues.empty())
                            f.prevHighPassValues.resize(NUM_CHANNELS, 0.0f);
                    }
                    else
                        f.prevHighPassValues.clear();

                    highPass = !f.prevHighPassValues.empty();

                    for (i = 0; i < BUFFER_SIZE_MONO; i++)
                    {
                        if (relativeOffset + i < f.automation[automationIndex].time)
                            interpolation = 0.0;
                        else
                            interpolation = std::clamp((relativeOffset + i - f.automation[automationIndex].time) / (double)(f.automation[automationIndex + 1].time - f.automation[automationIndex].time), 0.0, 1.0);

                        amplitude = interpolation * (f.automation[automationIndex + 1].amplitude - f.automation[automationIndex].amplitude) + f.automation[automationIndex].amplitude;

                        if (lowPass)
                            lowPassFrequency = interpolation * (f.automation[automationIndex + 1].lowPassFrequency - f.automation[automationIndex].lowPassFrequency) + f.automation[automationIndex].lowPassFrequency;
                        if (highPass)
                            highPassFrequency = interpolation * (f.automation[automationIndex + 1].highPassFrequency - f.automation[automationIndex].highPassFrequency) + f.automation[automationIndex].highPassFrequency;

                        for (c = 0; c < NUM_CHANNELS; c++)
                        {
                            float &dataValue = data[i * NUM_CHANNELS + c];

                            if (lowPass)
                            {
                                a1 = lc_audio::filter_a1(lowPassFrequency, SAMPLE_RATE);
                                out = lc_audio::filter_out(dataValue, a1, f.prevLowPassValues[c]);
                                f.prevLowPassValues[c] = lc_audio::filter_dn1(dataValue, a1, out);
                                dataValue = lc_audio::filter_lp(dataValue, out);
                            }
                            if (highPass)
                            {
                                a1 = lc_audio::filter_a1(highPassFrequency, SAMPLE_RATE);
                                out = lc_audio::filter_out(dataValue, a1, f.prevHighPassValues[c]);
                                f.prevHighPassValues[c] = lc_audio::filter_dn1(dataValue, a1, out);
                                dataValue = lc_audio::filter_hp(dataValue, out);
                            }

                            dataValue *= amplitude;
                        }
                    }
                }
            }
            else if (metronomeInterval == 0.0 && relativeOffset < f.duration - f.beats->getBeatInterval() / 2.0)
            {
                metronomeInterval = f.beats->getBeatInterval();
                metronomeOffset = (f.playOffset / D_SAMPLE_RATE) + f.beats->getBeatOffset();
            }

            if (bufferDataEmpty)
                bufferData = data;
            else
                for (i = 0; i < BUFFER_SIZE; i++)
                    bufferData[i] += data[i];

            bufferDataEmpty = false;
        }

    if (bufferDataEmpty)
        std::fill(bufferData.begin(), bufferData.end(), 0.0f);
    else if (!bounce)
        for (auto &sample : bufferData)
            sample *= filesVolume;

    if (!bounce && metronomeVolume != 0.0f && metronomeInterval != 0.0 && currentTime + BUFFER_SIZE_SECONDS > metronomeOffset)
    {
        double beatIndex = std::round((currentTime - metronomeOffset) / metronomeInterval),
               offsetTime = beatIndex * metronomeInterval + metronomeOffset - currentTime;

        if (offsetTime > -METRONOME_DURATION_SECONDS && offsetTime < BUFFER_SIZE_SECONDS)
        {
            long offset = offsetTime * D_SAMPLE_RATE, fileOffset{0};
            if (offset < 0)
            {
                fileOffset = -offset;
                offset = 0;
            }

            auto &data = metronomeAudioData->getData(fileOffset + METRONOME_DURATION * ((int)beatIndex % 4 == 0), METRONOME_DURATION - fileOffset, nullptr);
            const size_t size = std::min(BUFFER_SIZE_MONO - offset, data.size());

            for (size_t i = 0, c; i < size; i++)
                for (c = 0; c < NUM_CHANNELS; c++)
                    bufferData[(offset + i) * NUM_CHANNELS + c] += data[i] * metronomeVolume;
        }
    }

    bufferOffset += BUFFER_SIZE;

    if (!bounce)
    {
        bufferDataMutex.unlock();
        fileLoadingMutex.unlock();
    }
}