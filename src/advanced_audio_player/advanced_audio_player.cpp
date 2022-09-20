#include "advanced_audio_player/advanced_audio_player.hpp"

const std::string AdvancedAudioPlayer::CUSTOM_FILE_DATA_EXTENSION{".advanced_audio_player_file_data"},
    AdvancedAudioPlayer::CUSTOM_FILE_BUFFER_EXTENSION{".advanced_audio_player_file_buffer"};

AdvancedAudioPlayer::AdvancedAudioPlayer(std::string projectFolderPath)
    : lc_audio::Engine(2, std::pow(2, 13), 2, 44100, 0.3, 0.15),
      D_NUM_CHANNELS(NUM_CHANNELS), D_SAMPLE_RATE(SAMPLE_RATE), BUFFER_SIZE_MONO(BUFFER_SIZE / NUM_CHANNELS), BUFFER_SIZE_SECONDS(BUFFER_SIZE_MONO / D_SAMPLE_RATE),
      METRONOME_DURATION(SAMPLE_RATE / 10), METRONOME_DURATION_SECONDS(METRONOME_DURATION / D_SAMPLE_RATE),
      projectFolderPath(projectFolderPath)
{
    {
        const float fSampleRate = D_SAMPLE_RATE,
                    duration = METRONOME_DURATION_SECONDS;

        float angularVelocity[2]{lc_audio::angularVelocity(lc_audio::frequency<float>(5, lc_audio::midiNoteID("C"))), lc_audio::angularVelocity(lc_audio::frequency<float>(5, lc_audio::midiNoteID("D")))};

        std::vector<float> data(METRONOME_DURATION * 2);

        for (size_t i = 0, j; i < METRONOME_DURATION; i++)
        {
            float time{i / fSampleRate},
                amp{1.0f - time / duration};

            for (j = 0; j < 2; j++)
                data[j * METRONOME_DURATION + i] = amp * 0.7f * std::sin(angularVelocity[j] * time) + amp * 0.3f * std::sin(angularVelocity[j] * 0.5f * time);
        }

        metronomeAudioData = std::make_unique<lc_audio::File>(data, 1, SAMPLE_RATE);
    }

    std::vector<std::string> audioFilePaths;
    std::ifstream filePaths(projectFolderPath + "/paths.dat");
    std::string str;

    if (filePaths.is_open())
        while (!filePaths.eof())
        {
            std::getline(filePaths, str);
            if (!str.empty())
                audioFilePaths.push_back(str);
        }

    if (!audioFilePaths.empty())
        loadFiles(audioFilePaths);
}
AdvancedAudioPlayer::~AdvancedAudioPlayer()
{
    stopThread();

    if (fileThread)
    {
        stopFileLoading = true;
        fileThread->join();
    }
}

void AdvancedAudioPlayer::update()
{
    if (fileLoadingComplete || fileSavingComplete || filePitchChangeComplete || fileBounceComplete)
    {
        fileLoadingComplete = fileSavingComplete = filePitchChangeComplete = fileBounceComplete = false;
        updateFileThreadReset = true;
        fileThread->join();
        fileThread.reset();

        if (!initFilesLoaded)
        {
            initFilesLoaded = true;
            startThread();
        }
    }
    else
        updateFileThreadReset = false;
}

const bool &AdvancedAudioPlayer::fileThreadReset() const
{
    return updateFileThreadReset;
}

void AdvancedAudioPlayer::setVolume(float volume)
{
    this->filesVolume = std::clamp(volume, 0.0f, 1.0f);
}
const float &AdvancedAudioPlayer::getVolume() const
{
    return filesVolume;
}

void AdvancedAudioPlayer::setMetronomeVolume(float metronomeVolume)
{
    this->metronomeVolume = std::clamp(metronomeVolume, 0.0f, 1.0f);
}
const float &AdvancedAudioPlayer::getMetronomeVolume() const
{
    return metronomeVolume;
}
const double &AdvancedAudioPlayer::getCurrentTime() const
{
    return currentTime;
}
void AdvancedAudioPlayer::setCurrentTime(double time)
{
    bufferDataMutex.lock();

    bufferOffset = time * D_NUM_CHANNELS * D_SAMPLE_RATE;
    if (bufferOffset % 2 == 1)
        bufferOffset -= 1;
    currentTime = time;

    bufferDataMutex.unlock();
}

const float &AdvancedAudioPlayer::getProgress() const
{
    return progress;
}

void AdvancedAudioPlayer::saveFiles()
{
    progress = 0.0f;
    fileSavingComplete = false;
    fileThread = std::make_unique<std::thread>(fileSavingThread_func, this);
}

void AdvancedAudioPlayer::swapFileLocations(size_t index1, size_t index2)
{
    if (fileThread)
        throw std::runtime_error("AdvancedAudioPlayer::swapFileLocations Cannot modify files when file loading thread is active!");

    if (index1 >= files.size())
        throw std::invalid_argument("AdvancedAudioPlayer::swapFileLocations Invalid index1: " + std::to_string(index1));

    if (index2 >= files.size())
        throw std::invalid_argument("AdvancedAudioPlayer::swapFileLocations Invalid index2: " + std::to_string(index2));

    if (index1 != index2)
        std::swap(files[index1], files[index2]);
}
void AdvancedAudioPlayer::removeFiles(size_t startIndex, size_t numFiles)
{
    if (fileThread)
        throw std::runtime_error("AdvancedAudioPlayer::removeFiles Cannot modify files when file loading thread is active!");

    if (startIndex >= files.size())
        throw std::invalid_argument("AdvancedAudioPlayer::removeFiles Invalid startIndex: " + std::to_string(startIndex));

    fileLoadingMutex.lock();

    files.erase(files.begin() + startIndex, files.begin() + startIndex + numFiles);

    fileLoadingMutex.unlock();
}

void AdvancedAudioPlayer::loadFiles(std::vector<std::string> audioFilePaths)
{
    progress = 0.0f;
    fileLoadingComplete = false;
    stopFileLoading = false;
    fileThread = std::make_unique<std::thread>(fileLoadingThread_func, this, audioFilePaths);
}

void AdvancedAudioPlayer::bounceFiles(const std::string &filePath)
{
    progress = 0.0f;
    fileBounceComplete = false;
    fileThread = std::make_unique<std::thread>(fileBounceThread_func, this, filePath);
}

float AdvancedAudioPlayer::exponentialFrequencyRange(float frequency)
{
    constexpr float exponent = 0.3f;
    return std::pow((frequency - MIN_FREQUENCY) / (MAX_FREQUENCY - MIN_FREQUENCY), exponent);
}
float AdvancedAudioPlayer::exponentialFrequencyRangeInverse(float range)
{
    constexpr float exponent = 1.0f / 0.3f;
    return std::pow(range, exponent) * (MAX_FREQUENCY - MIN_FREQUENCY) + MIN_FREQUENCY;
}