#include "advanced_audio_player/advanced_audio_player.hpp"

void AdvancedAudioPlayer::fileLoadingThread_func(AdvancedAudioPlayer *instance, std::vector<std::string> audioFilePaths)
{
    instance->pause();
    instance->fileLoadingMutex.lock();
    float progressStep = 1.0f / audioFilePaths.size();

    instance->files.reserve(instance->files.size() + audioFilePaths.size());

    for (auto &str : audioFilePaths)
    {
        if (instance->stopFileLoading)
            break;

        size_t lastSlash = str.find_last_of('/') + 1;
        std::string fileName = str.substr(lastSlash, str.find_last_of('.') - lastSlash),
                    customFilePath = instance->projectFolderPath + '/' + fileName,
                    customFileDataPath = customFilePath + CUSTOM_FILE_DATA_EXTENSION,
                    customFileBufferPath = customFilePath + CUSTOM_FILE_BUFFER_EXTENSION;

        instance->files.push_back(FileData());

        if (std::filesystem::exists(customFileDataPath) && std::filesystem::exists(customFileBufferPath))
        {
            std::vector<float> data;
            unsigned int numChannels, sampleRate, bpm;
            double beatOffset;

            custom_file::load(customFileDataPath, customFileBufferPath, numChannels, sampleRate, bpm, beatOffset, instance->files.back().playOffset, instance->files.back().automation, data);

            instance->files.back().file = std::make_unique<lc_audio::File>(data, numChannels, sampleRate);
            instance->files.back().beats = std::make_unique<lc_audio::Beats>(bpm, beatOffset);
        }
        else
        {
            instance->files.back().file = std::make_unique<lc_audio::File>(str);

            instance->files.back().file->setNumChannels(instance->NUM_CHANNELS);
            instance->files.back().file->setSampleRate(instance->SAMPLE_RATE);
            instance->files.back().file->normalize(-14.0f);

            instance->files.back().beats = std::make_unique<lc_audio::Beats>(instance->files.back().file->getFullBuffer(), instance->NUM_CHANNELS, instance->SAMPLE_RATE);

            instance->files.back().playOffset = instance->files.size() > 1 ? instance->files[instance->files.size() - 2].playOffset + instance->files[instance->files.size() - 2].duration : 0;
            instance->files.back().bufferDataHasChanged = true;
        }

        instance->files.back().duration = instance->files.back().file->getSize() / instance->NUM_CHANNELS;
        instance->files.back().name = fileName;
        instance->files.back().filePath = str;

        instance->progress += progressStep;
    }

    instance->fileLoadingComplete = true;
    instance->fileLoadingMutex.unlock();
}

void AdvancedAudioPlayer::fileSavingThread_func(AdvancedAudioPlayer *instance)
{
    float progressStep = 1.0f / instance->files.size();

    std::ofstream projectFilePaths(instance->projectFolderPath + "/paths.dat");
    if (!projectFilePaths.is_open())
        return;

    bool firstFile = true;

    for (auto &f : instance->files)
    {
        if (!firstFile)
            projectFilePaths << '\n';

        projectFilePaths << f.filePath;

        std::string filePath = instance->projectFolderPath + '/' + f.name,
                    dataPath = filePath + CUSTOM_FILE_DATA_EXTENSION,
                    bufferPath = filePath + CUSTOM_FILE_BUFFER_EXTENSION;

        custom_file::save(dataPath, bufferPath, instance->NUM_CHANNELS, instance->SAMPLE_RATE, f.beats->getBPM(), f.beats->getBeatOffset(), f.playOffset, f.automation, f.file->getFullBuffer(), f.bufferDataHasChanged);
        f.bufferDataHasChanged = false;

        instance->progress += progressStep;
        firstFile = false;
    }

    projectFilePaths.close();

    instance->fileSavingComplete = true;
}

void AdvancedAudioPlayer::filePitchChangeThread_func(AdvancedAudioPlayer *instance, size_t index, double pitch)
{
    instance->pause();
    instance->fileLoadingMutex.lock();

    instance->files[index].file->pitchChange(pitch);

    instance->files[index].duration = instance->files[index].file->getSize() / instance->NUM_CHANNELS;

    double prevBeatOffset = instance->files[index].beats->getBeatOffset();
    instance->files[index].beats->setBPM(instance->files[index].beats->getBPM() * pitch);
    instance->files[index].beats->setBeatOffset(prevBeatOffset / pitch);

    for (auto &i : instance->files[index].automation)
        i.time /= pitch;

    instance->files[index].playOffset = index == 0 ? 0 : instance->files[index - 1].playOffset + instance->files[index - 1].duration;

    instance->progress = 1.0f;
    instance->filePitchChangeComplete = true;
    instance->fileLoadingMutex.unlock();
}

void AdvancedAudioPlayer::fileBounceThread_func(AdvancedAudioPlayer *instance, std::string filePath)
{
    instance->pause();
    instance->fileLoadingMutex.lock();

    size_t outputSize{0};
    for (auto &f : instance->files)
        outputSize += f.file->getSize();

    std::vector<float> outputData(outputSize + instance->BUFFER_SIZE);

    float progressStep = instance->BUFFER_SIZE / (float)outputSize * 0.9f;

    size_t prevBufferOffset = instance->bufferOffset;
    instance->bufferOffset = 0;
    size_t index{0}, i, c;

    while (index < outputSize)
    {
        instance->progress += progressStep;
        instance->advanced_updateBufferData(true);

        for (i = 0; i < instance->BUFFER_SIZE; i++)
            outputData[index + i] = instance->bufferData[i];

        index = instance->bufferOffset;
    }

    bool b_break{false};

    for (i = 0;; i++)
    {
        for (c = 0; c < instance->NUM_CHANNELS; c++)
            if (std::abs(outputData[i * instance->NUM_CHANNELS + c]) > 0.001f)
            {
                index = i * instance->NUM_CHANNELS - instance->NUM_CHANNELS;
                b_break = true;
                break;
            }

        if (b_break)
            break;
    }

    outputData.erase(outputData.begin(), outputData.begin() + index);

    b_break = false;

    for (i = outputSize / 2 - 1;; i--)
    {
        for (c = 0; c < instance->NUM_CHANNELS; c++)
            if (std::abs(outputData[i * instance->NUM_CHANNELS + c]) > 0.001f)
            {
                index = i * instance->NUM_CHANNELS + instance->NUM_CHANNELS;
                b_break = true;
                break;
            }

        if (b_break)
            break;
    }

    outputData.resize(index);

    lc_audio::File::write(filePath, outputData, instance->NUM_CHANNELS);

    instance->bufferOffset = prevBufferOffset;
    instance->currentTime = prevBufferOffset / instance->NUM_CHANNELS / instance->D_SAMPLE_RATE;

    instance->progress = 1.0f;
    instance->fileBounceComplete = true;
    instance->fileLoadingMutex.unlock();
}