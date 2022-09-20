#include "advanced_audio_player/advanced_audio_player.hpp"

void AdvancedAudioPlayer::setFilePlayOffset(size_t offset, size_t index)
{
    if (fileThread)
        throw std::runtime_error("AdvancedAudioPlayer::setFilePlayOffset Cannot modify files when file loading thread is active!");

    if (index >= files.size())
        throw std::invalid_argument("AdvancedAudioPlayer::setFilePlayOffset Invalid index: " + std::to_string(index));

    files[index].playOffset = offset;
}
void AdvancedAudioPlayer::setFilePlayOffset(double offset, size_t index)
{
    setFilePlayOffset((size_t)(offset * SAMPLE_RATE), index);
}

void AdvancedAudioPlayer::setFileBPM(unsigned int bpm, size_t index)
{
    if (fileThread)
        throw std::runtime_error("AdvancedAudioPlayer::setFileBPM Cannot modify files when file loading thread is active!");

    if (index >= files.size())
        throw std::invalid_argument("AdvancedAudioPlayer::setFileBPM Invalid index: " + std::to_string(index));

    if (bpm == files[index].beats->getBPM())
        return;

    files[index].bufferDataHasChanged = true;

    double pitch = bpm / (double)files[index].beats->getBPM();

    progress = 0.0f;
    filePitchChangeComplete = false;
    fileThread = std::make_unique<std::thread>(filePitchChangeThread_func, this, index, pitch);
}

void AdvancedAudioPlayer::mute(size_t index)
{
    if (fileThread)
        throw std::runtime_error("AdvancedAudioPlayer::mute Cannot modify files when file loading thread is active!");

    if (index >= files.size())
        throw std::invalid_argument("AdvancedAudioPlayer::mute Invalid index: " + std::to_string(index));

    files[index].muted = true;
}
void AdvancedAudioPlayer::unmute(size_t index)
{
    if (fileThread)
        throw std::runtime_error("AdvancedAudioPlayer::unmute Cannot modify files when file loading thread is active!");

    if (index >= files.size())
        throw std::invalid_argument("AdvancedAudioPlayer::unmute Invalid index: " + std::to_string(index));

    files[index].muted = false;
}