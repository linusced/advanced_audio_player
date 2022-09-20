#include "advanced_audio_player/advanced_audio_player.hpp"

void AdvancedAudioPlayer::custom_file::load(const std::string &fileDataPath, const std::string &fileBufferPath, unsigned int &numChannels, unsigned int &sampleRate, unsigned int &bpm, double &beatOffset, size_t &playOffset, std::vector<Keyframe> &automation, std::vector<float> &buffer)
{
    lc_utility::BinaryFileInput dataIn{fileDataPath};

    numChannels = dataIn.read<uint32_t>();
    sampleRate = dataIn.read<uint32_t>();
    bpm = dataIn.read<uint32_t>();
    beatOffset = dataIn.read<double>();
    playOffset = dataIn.read<uint64_t>();

    size_t automationArrSize = dataIn.read<uint64_t>();

    if (automationArrSize != 0)
    {
        automation.resize(automationArrSize, Keyframe(0));
        dataIn.read(automation.data(), automationArrSize);
    }

    lc_utility::BinaryFileInput bufferIn{fileBufferPath};

    size_t arrSize = bufferIn.read<uint64_t>();
    std::vector<int16_t> intBuffer(arrSize);
    bufferIn.read(intBuffer.data(), arrSize);

    buffer.resize(arrSize);
    for (size_t i = 0; i < arrSize; i++)
        buffer[i] = lc_audio::int16ToSample<float>(intBuffer[i]);
}

void AdvancedAudioPlayer::custom_file::save(const std::string &fileDataPath, const std::string &fileBufferPath, unsigned int numChannels, unsigned int sampleRate, unsigned int bpm, double beatOffset, size_t playOffset, const std::vector<Keyframe> &automation, const std::vector<float> &buffer, bool bufferDataHasChanged)
{
    lc_utility::BinaryFileOutput dataOut{fileDataPath};

    dataOut.write((uint32_t)numChannels);
    dataOut.write((uint32_t)sampleRate);
    dataOut.write((uint32_t)bpm);
    dataOut.write(beatOffset);
    dataOut.write((uint64_t)playOffset);

    size_t automationArrSize = automation.size();
    dataOut.write((uint64_t)automationArrSize);

    if (automationArrSize != 0)
        dataOut.write(automation.data(), automation.size());

    if (bufferDataHasChanged)
    {
        size_t arrSize = buffer.size();
        std::vector<int16_t> intBuffer(arrSize);
        for (size_t i = 0; i < arrSize; i++)
            intBuffer[i] = lc_audio::sampleToInt16(buffer[i]);

        lc_utility::BinaryFileOutput bufferOut{fileBufferPath};

        bufferOut.write((uint64_t)arrSize);
        bufferOut.write(intBuffer.data(), arrSize);
    }
}