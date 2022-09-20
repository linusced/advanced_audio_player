#include "audio_player_gui/waveform.hpp"

const glm::u8vec3 audio_player_gui::Waveform::playHeadColor{228, 41, 163},
    audio_player_gui::Waveform::waveformColor{255, 255, 255}, audio_player_gui::Waveform::beatColor{127, 0, 64},
    audio_player_gui::Waveform::amplitudeColor{255, 127, 0},
    audio_player_gui::Waveform::lowPassColor{22, 127, 232}, audio_player_gui::Waveform::highPassColor{22, 57, 232},
    audio_player_gui::Waveform::automationKeyframePosColor{35, 205, 145}, audio_player_gui::Waveform::automationKeyframePosActiveColor{205, 180, 35};

audio_player_gui::Waveform::Waveform(size_t textureWidth, size_t textureHeight, size_t bufferNumChannels, size_t bufferSampleRate, const std::string &className)
    : lc_gui::Element(className), BUFFER_NUM_CHANNELS(bufferNumChannels), BUFFER_SAMPLE_RATE(bufferSampleRate), D_BUFFER_SAMPLE_RATE(BUFFER_SAMPLE_RATE)
{
    mesh.vertices.resize(4);
    mesh.indices = {0, 1, 2, 0, 2, 3};

    for (auto &v : mesh.vertices)
        v.color = glm::vec4(playHeadColor.r / 255.0f, playHeadColor.g / 255.0f, playHeadColor.b / 255.0f, 1.0f);

    extraMeshes.push_back(std::make_unique<lc_gui::Mesh2D>());
    extraMeshes[0]->vertices.resize(4);
    extraMeshes[0]->indices = mesh.indices;

    extraMeshes[0]->vertices[0].textureCoords = {0.0f, 0.0f};
    extraMeshes[0]->vertices[1].textureCoords = {1.0f, 0.0f};
    extraMeshes[0]->vertices[2].textureCoords = {1.0f, 1.0f};
    extraMeshes[0]->vertices[3].textureCoords = {0.0f, 1.0f};

    textureData.format = GL_RGB;
    textureData.width = textureWidth;
    textureData.height = textureHeight;
    textureData.bytes.resize(textureData.width * textureData.height * 3, 0);

    texture = std::make_unique<lc_gui::Texture>(textureData.bytes, textureData.width, textureData.height, textureData.format);
    extraMeshes[0]->texture = texture.get();
}

void audio_player_gui::Waveform::resizeMesh(const lc_gui::Style &style, const lc_gui::Layout &layout, const glm::ivec2 &windowSize)
{
    extraMeshes[0]->vertices[0].position = {layout.x, layout.y};
    extraMeshes[0]->vertices[1].position = {layout.x + layout.width, layout.y};
    extraMeshes[0]->vertices[2].position = {layout.x + layout.width, layout.y + layout.height};
    extraMeshes[0]->vertices[3].position = {layout.x, layout.y + layout.height};

    int playHeadPosPixels = playHeadPos * layout.width, playHeadHeight = (playHeadPosPixels >= 0 && playHeadPosPixels < layout.width) ? layout.height : 0;

    mesh.vertices[0].position = {layout.x + playHeadPosPixels - 2, layout.y};
    mesh.vertices[1].position = {layout.x + playHeadPosPixels + 2, layout.y};
    mesh.vertices[2].position = {layout.x + playHeadPosPixels + 2, layout.y + playHeadHeight};
    mesh.vertices[3].position = {layout.x + playHeadPosPixels - 2, layout.y + playHeadHeight};

    for (auto &v : mesh.vertices)
        v.position /= windowSize;

    mesh.applyVertexChange();

    for (auto &v : extraMeshes[0]->vertices)
        v.position /= windowSize;

    extraMeshes[0]->applyVertexChange();
}

void audio_player_gui::Waveform::setNumWaveforms(size_t numWaveforms)
{
    waveforms.resize(numWaveforms);
}
void audio_player_gui::Waveform::setData(const std::vector<float> *bufferData, std::vector<AdvancedAudioPlayer::Keyframe> *automation, size_t waveformIndex)
{
    waveforms[waveformIndex].data = bufferData;
    waveforms[waveformIndex].automation = automation;
    waveforms[waveformIndex].bufferSizePerChannels = bufferData->size() / BUFFER_NUM_CHANNELS;
    waveforms[waveformIndex].maxTime = waveforms[waveformIndex].bufferSizePerChannels / D_BUFFER_SAMPLE_RATE;

    totalSize = 0;
    for (auto &w : waveforms)
        totalSize += w.bufferSizePerChannels;

    zoom = totalSize;
    maxTime = totalSize / D_BUFFER_SAMPLE_RATE;
    panning = 0;
}
void audio_player_gui::Waveform::setBeats(double beatInterval, double beatOffset, size_t waveformIndex)
{
    waveforms[waveformIndex].beatInterval = beatInterval;
    waveforms[waveformIndex].beatOffset = beatOffset;
}
void audio_player_gui::Waveform::setOffset(size_t offset, size_t waveformIndex)
{
    waveforms[waveformIndex].offset = offset;
}

void audio_player_gui::Waveform::setPlayHead(double time)
{
    if (maxTime != 0.0f && time != prevPlayHeadTime)
    {
        playHeadPos = (time - panning / D_BUFFER_SAMPLE_RATE) / (zoom / (double)totalSize * maxTime);
        prevPlayHeadTime = time;
        styleChange = true;
    }
}

const double &audio_player_gui::Waveform::getMaxTime() const
{
    return maxTime;
}

void audio_player_gui::Waveform::setLmbType(LMB_TYPE type)
{
    bool b_drawWaveforms = false;
    if (waveformDragIndex != -1)
    {
        waveforms[waveformDragIndex].offset = waveformDragPreGlobalOffset;
        waveformDragIndex = -1;
        b_drawWaveforms = true;
    }

    if (selectedKeyframeIndex != -1)
    {
        if (lmbType == LMB_MOVE_KEYFRAME)
        {
            auto &a = waveforms[selectedKeyframeWaveIndex].automation;
            int offset = selectedKeyframeIndex < selectedKeyframeInitIndex ? 1 : -1;

            for (; selectedKeyframeIndex != selectedKeyframeInitIndex; selectedKeyframeIndex += offset)
                std::swap(a->at(selectedKeyframeIndex), a->at(selectedKeyframeIndex + offset));

            a->at(selectedKeyframeIndex).time = selectedKeyframePreMoveTime;
        }
        else if (type == LMB_MOVE_KEYFRAME)
        {
            selectedKeyframePreMoveTime = waveforms[selectedKeyframeWaveIndex].automation->at(selectedKeyframeIndex).time;
            selectedKeyframeInitIndex = selectedKeyframeIndex;
            onKeyframeClear();
        }
        else
        {
            selectedKeyframeWaveIndex = selectedKeyframeIndex = -1;
            onKeyframeClear();
        }

        b_drawWaveforms = true;
    }

    if (createKeyframeWaveIndex != -1)
    {
        createKeyframeWaveIndex = -1;
        b_drawWaveforms = true;
    }

    if (lmbType != type)
        onLmbTypeChange(type);

    lmbType = type;

    if (b_drawWaveforms)
        drawWaveforms();
}

bool audio_player_gui::Waveform::inputs(const glm::ivec2 &relativeCursorPosition)
{
    if (maxTime == 0.0)
        return false;

    glm::ivec2 relativeCursorPositionOffset = relativeCursorPosition;
    if (prevLayout.y < 0)
    {
        relativeCursorPositionOffset.y -= prevLayout.y;
        if (relativeCursorPositionOffset.y >= prevLayout.height)
            return false;
    }

    int cursorWaveIndex = std::clamp((int)(relativeCursorPositionOffset.y / (float)prevLayout.height * waveforms.size()), 0, (int)waveforms.size() - 1),
        cursorKeyframeIndex = -1;
    long cursorWaveRelativePos = (relativeCursorPositionOffset.x / (double)prevLayout.width) * zoom + panning - waveforms[cursorWaveIndex].offset;

    inputsSetCursor(cursorWaveIndex, cursorKeyframeIndex, cursorWaveRelativePos);

    bool b_drawWaveforms = false;
    float cursorClampedX = std::clamp(relativeCursorPositionOffset.x / (double)prevLayout.width, 0.0, 1.0),
          cursorTime = cursorClampedX * (zoom / (double)totalSize * maxTime) + panning / D_BUFFER_SAMPLE_RATE;

    inputsLmbType(relativeCursorPositionOffset, cursorWaveIndex, cursorWaveRelativePos, b_drawWaveforms);
    inputsLmbDown(cursorWaveIndex, cursorKeyframeIndex, cursorWaveRelativePos, cursorTime, b_drawWaveforms);
    inputsEscapeKey(b_drawWaveforms);
    inputsZoomAndPanning(relativeCursorPositionOffset, cursorClampedX, cursorTime, b_drawWaveforms);

    if (b_drawWaveforms)
        drawWaveforms();

    requireInputs = lmbDown || mmbDown;
    return requireInputs;
}