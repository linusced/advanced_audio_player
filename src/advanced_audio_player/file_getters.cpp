#include "advanced_audio_player/advanced_audio_player.hpp"

size_t AdvancedAudioPlayer::getNumFiles() const
{
    if (fileThread)
        throw std::runtime_error("AdvancedAudioPlayer::getNumFiles Cannot access files when file loading thread is active!");

    return files.size();
}

lc_audio::File *AdvancedAudioPlayer::getFile(size_t index)
{
    if (fileThread)
        throw std::runtime_error("AdvancedAudioPlayer::getFile Cannot access files when file loading thread is active!");

    if (index >= files.size())
        throw std::invalid_argument("AdvancedAudioPlayer::getFile Invalid index: " + std::to_string(index));

    return files[index].file.get();
}
lc_audio::Beats *AdvancedAudioPlayer::getFileBeats(size_t index)
{
    if (fileThread)
        throw std::runtime_error("AdvancedAudioPlayer::getFileBeats Cannot access files when file loading thread is active!");

    if (index >= files.size())
        throw std::invalid_argument("AdvancedAudioPlayer::getFileBeats Invalid index: " + std::to_string(index));

    return files[index].beats.get();
}

const std::string &AdvancedAudioPlayer::getFileName(size_t index) const
{
    if (fileThread)
        throw std::runtime_error("AdvancedAudioPlayer::getFileName Cannot access files when file loading thread is active!");

    if (index >= files.size())
        throw std::invalid_argument("AdvancedAudioPlayer::getFileName Invalid index: " + std::to_string(index));

    return files[index].name;
}

const std::string &AdvancedAudioPlayer::getFilePath(size_t index) const
{
    if (fileThread)
        throw std::runtime_error("AdvancedAudioPlayer::getFilePath Cannot access files when file loading thread is active!");

    if (index >= files.size())
        throw std::invalid_argument("AdvancedAudioPlayer::getFilePath Invalid index: " + std::to_string(index));

    return files[index].filePath;
}

const size_t &AdvancedAudioPlayer::getFileDuration(size_t index) const
{
    if (fileThread)
        throw std::runtime_error("AdvancedAudioPlayer::getFileDuration Cannot access files when file loading thread is active!");

    if (index >= files.size())
        throw std::invalid_argument("AdvancedAudioPlayer::getFileDuration Invalid index: " + std::to_string(index));

    return files[index].duration;
}
const size_t &AdvancedAudioPlayer::getFilePlayOffset(size_t index) const
{
    if (fileThread)
        throw std::runtime_error("AdvancedAudioPlayer::getFilePlayOffset Cannot access files when file loading thread is active!");

    if (index >= files.size())
        throw std::invalid_argument("AdvancedAudioPlayer::getFilePlayOffset Invalid index: " + std::to_string(index));

    return files[index].playOffset;
}
double AdvancedAudioPlayer::getFileDuration_d(size_t index) const
{
    if (fileThread)
        throw std::runtime_error("AdvancedAudioPlayer::getFileDuration_d Cannot access files when file loading thread is active!");

    if (index >= files.size())
        throw std::invalid_argument("AdvancedAudioPlayer::getFileDuration_d Invalid index: " + std::to_string(index));

    return files[index].duration / D_SAMPLE_RATE;
}
double AdvancedAudioPlayer::getFilePlayOffset_d(size_t index) const
{
    if (fileThread)
        throw std::runtime_error("AdvancedAudioPlayer::getFilePlayOffset_d Cannot access files when file loading thread is active!");

    if (index >= files.size())
        throw std::invalid_argument("AdvancedAudioPlayer::getFilePlayOffset_d Invalid index: " + std::to_string(index));

    return files[index].playOffset / D_SAMPLE_RATE;
}

std::vector<AdvancedAudioPlayer::Keyframe> *AdvancedAudioPlayer::getFileAutomation(size_t index)
{
    if (fileThread)
        throw std::runtime_error("AdvancedAudioPlayer::getFileAutomation Cannot access files when file loading thread is active!");

    if (index >= files.size())
        throw std::invalid_argument("AdvancedAudioPlayer::getFileAutomation Invalid index: " + std::to_string(index));

    return &files[index].automation;
}

const bool &AdvancedAudioPlayer::isMuted(size_t index) const
{
    if (fileThread)
        throw std::runtime_error("AdvancedAudioPlayer::isMuted Cannot access files when file loading thread is active!");

    if (index >= files.size())
        throw std::invalid_argument("AdvancedAudioPlayer::isMuted Invalid index: " + std::to_string(index));

    return files[index].muted;
}