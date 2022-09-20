#include "audio_player_gui/waveform.hpp"

void audio_player_gui::Waveform::inputsLmbDown(const int &cursorWaveIndex, const int &cursorKeyframeIndex, const long &cursorWaveRelativePos, const float &cursorTime, bool &b_drawWaveforms)
{
    if (window->isLmbDown())
    {
        if (!lmbDown)
        {
            lmbDown = true;

            if (lmbType == LMB_TIME_CHANGE)
            {
                onTimeChange(cursorTime);
            }
            else if (lmbType == LMB_MOVE_WAVEFORM)
            {
                if (waveformDragIndex == -1)
                {
                    waveformDragIndex = cursorWaveIndex;
                    if (waveformDragIndex == 0)
                    {
                        waveformDragIndex = -1;
                    }
                    else
                    {
                        if (cursorWaveRelativePos >= 0 && cursorWaveRelativePos < waveforms[waveformDragIndex].bufferSizePerChannels)
                            waveformDragOffset = cursorWaveRelativePos;
                        else
                            waveformDragIndex = -1;

                        waveformDragPreGlobalOffset = waveforms[waveformDragIndex].offset;
                    }
                }
                else
                {
                    onWaveformMove(waveforms[waveformDragIndex].offset, waveformDragIndex);
                    waveformDragIndex = -1;
                    b_drawWaveforms = true;
                }
            }
            else if (lmbType == LMB_SELECT_KEYFRAME && cursorKeyframeIndex != -1)
            {
                selectedKeyframeWaveIndex = cursorWaveIndex;
                selectedKeyframeIndex = cursorKeyframeIndex;
                onKeyframeSelect(&waveforms[selectedKeyframeWaveIndex].automation->at(selectedKeyframeIndex), selectedKeyframeWaveIndex);
                b_drawWaveforms = true;
            }
            else if (lmbType == LMB_MOVE_KEYFRAME)
            {
                if (selectedKeyframeIndex == -1)
                {
                    if (cursorKeyframeIndex != -1)
                    {
                        selectedKeyframeWaveIndex = cursorWaveIndex;
                        selectedKeyframeIndex = selectedKeyframeInitIndex = cursorKeyframeIndex;
                        selectedKeyframePreMoveTime = waveforms[selectedKeyframeWaveIndex].automation->at(selectedKeyframeIndex).time;
                    }
                }
                else
                {
                    lmbType = LMB_SELECT_KEYFRAME;
                    onLmbTypeChange(lmbType);
                    onKeyframeSelect(&waveforms[selectedKeyframeWaveIndex].automation->at(selectedKeyframeIndex), selectedKeyframeWaveIndex);
                }

                b_drawWaveforms = true;
            }
            else if (lmbType == LMB_CREATE_KEYFRAME)
            {
                auto &a = waveforms[cursorWaveIndex].automation;
                size_t index;

                if (a->empty())
                {
                    a->push_back(AdvancedAudioPlayer::Keyframe(createKeyframeTime));
                    index = 0;
                }
                else if (createKeyframeTime < a->front().time)
                {
                    AdvancedAudioPlayer::Keyframe kf(createKeyframeTime, a->front().amplitude, a->front().lowPassFrequency, a->front().highPassFrequency);
                    a->insert(a->begin(), kf);
                    index = 0;
                }
                else if (createKeyframeTime > a->back().time)
                {
                    AdvancedAudioPlayer::Keyframe kf(createKeyframeTime, a->back().amplitude, a->back().lowPassFrequency, a->back().highPassFrequency);
                    a->push_back(kf);
                    index = a->size() - 1;
                }
                else
                    for (size_t i = 1; i < a->size(); i++)
                        if (createKeyframeTime > a->at(i - 1).time && createKeyframeTime < a->at(i).time)
                        {
                            double interpolation = (createKeyframeTime - a->at(i - 1).time) / (double)(a->at(i).time - a->at(i - 1).time);

                            AdvancedAudioPlayer::Keyframe kf(createKeyframeTime);
                            kf.amplitude = interpolation * (a->at(i).amplitude - a->at(i - 1).amplitude) + a->at(i - 1).amplitude;
                            kf.lowPassFrequency = interpolation * (a->at(i).lowPassFrequency - a->at(i - 1).lowPassFrequency) + a->at(i - 1).lowPassFrequency;
                            kf.highPassFrequency = interpolation * (a->at(i).highPassFrequency - a->at(i - 1).highPassFrequency) + a->at(i - 1).highPassFrequency;

                            a->insert(a->begin() + i, kf);
                            index = i;
                            break;
                        }

                lmbType = LMB_SELECT_KEYFRAME;
                onLmbTypeChange(lmbType);
                createKeyframeWaveIndex = -1;
                selectedKeyframeWaveIndex = cursorWaveIndex;
                selectedKeyframeIndex = index;
                onKeyframeSelect(&waveforms[selectedKeyframeWaveIndex].automation->at(selectedKeyframeIndex), selectedKeyframeWaveIndex);

                b_drawWaveforms = true;
            }
        }
    }
    else
        lmbDown = false;
}