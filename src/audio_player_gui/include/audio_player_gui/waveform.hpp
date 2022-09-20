#pragma once

#include "lc_gui/lc_gui.hpp"
#include "advanced_audio_player/advanced_audio_player.hpp"

namespace audio_player_gui
{
    class Waveform : public lc_gui::Element
    {
    public:
        Waveform(size_t textureWidth, size_t textureHeight, size_t bufferNumChannels, size_t bufferSampleRate, const std::string &className);

        void resizeMesh(const lc_gui::Style &style, const lc_gui::Layout &layout, const glm::ivec2 &windowSize) override;
        bool inputs(const glm::ivec2 &relativeCursorPosition) override;

        void setNumWaveforms(size_t numWaveforms);
        void setData(const std::vector<float> *bufferData, std::vector<AdvancedAudioPlayer::Keyframe> *automation, size_t waveformIndex);
        void setBeats(double beatInterval, double beatOffset, size_t waveformIndex);
        void setOffset(size_t offset, size_t waveformIndex);

        const double &getMaxTime() const;

        void setPlayHead(double time);

        enum LMB_TYPE
        {
            LMB_TIME_CHANGE,
            LMB_MOVE_WAVEFORM,
            LMB_SELECT_KEYFRAME,
            LMB_MOVE_KEYFRAME,
            LMB_CREATE_KEYFRAME
        };
        void setLmbType(LMB_TYPE type);

        std::function<void(LMB_TYPE)> onLmbTypeChange;
        std::function<void(double)> onTimeChange;
        std::function<void(size_t, size_t)> onWaveformMove;
        std::function<void(AdvancedAudioPlayer::Keyframe *, size_t)> onKeyframeSelect;
        std::function<void()> onKeyframeClear;

        void drawWaveforms();

    private:
        std::unique_ptr<lc_gui::Texture> texture;
        lc_gui::TextureData textureData;

        const size_t BUFFER_NUM_CHANNELS, BUFFER_SAMPLE_RATE;
        const double D_BUFFER_SAMPLE_RATE;

        static const glm::u8vec3 playHeadColor, waveformColor, beatColor, amplitudeColor, lowPassColor, highPassColor, automationKeyframePosColor, automationKeyframePosActiveColor;

        double playHeadPos{-1.0}, prevPlayHeadTime{-1.0},
            maxTime{0.0};
        long zoom, panning, totalSize;

        struct _waveform
        {
            const std::vector<float> *data{nullptr};
            std::vector<AdvancedAudioPlayer::Keyframe> *automation{nullptr};
            size_t bufferSizePerChannels{0}, offset{0};
            double maxTime{0.0}, beatInterval{0.0}, beatOffset{0.0};
        };
        std::vector<_waveform> waveforms;

        bool mmbDown{false}, escapeDown{false};
        int mmbDownXOffset, waveformDragIndex{-1}, selectedKeyframeWaveIndex{-1}, selectedKeyframeIndex{-1}, selectedKeyframeInitIndex, createKeyframeWaveIndex{-1};
        size_t waveformDragOffset, waveformDragPreGlobalOffset, selectedKeyframePreMoveTime, createKeyframeTime;

        LMB_TYPE lmbType{LMB_TIME_CHANGE};

        void drawPixel(int x, int y, int colorIndex);
        void drawLine(glm::ivec2 begin, glm::ivec2 end, int colorIndex);

        void inputsSetCursor(const int &cursorWaveIndex, int &cursorKeyframeIndex, const long &cursorWaveRelativePos);
        void inputsLmbType(const glm::ivec2 &relativeCursorPosition, const int &cursorWaveIndex, const long &cursorWaveRelativePos, bool &b_drawWaveforms);
        void inputsLmbDown(const int &cursorWaveIndex, const int &cursorKeyframeIndex, const long &cursorWaveRelativePos, const float &cursorTime, bool &b_drawWaveforms);
        void inputsEscapeKey(bool &b_drawWaveforms);
        void inputsZoomAndPanning(const glm::ivec2 &relativeCursorPosition, const float &cursorClampedX, const float &cursorTime, bool &b_drawWaveforms);
    };
}