#include "advanced_audio_player/advanced_audio_player.hpp"

void AdvancedAudioPlayer::custom_file::load(const std::string &fileDataPath, const std::string &fileBufferPath, unsigned int &numChannels, unsigned int &sampleRate, unsigned int &bpm, double &beatOffset, size_t &playOffset, std::vector<Keyframe> &automation, std::vector<float> &buffer)
{
    std::vector<int16_t> intBuffer;
    size_t arrSize;

    lc_utility::BinaryFileRead dataIn{fileDataPath};

    uint16_t fileVersion = dataIn.read<uint16_t>();

    if (fileVersion == 1)
    {
        numChannels = dataIn.read<uint8_t>();
        sampleRate = dataIn.read<uint32_t>();
        bpm = dataIn.read<uint32_t>();
        beatOffset = dataIn.read<double>();
        playOffset = dataIn.read<uint64_t>();

        automation.resize(dataIn.read<uint64_t>(), Keyframe(0));
        dataIn.read(automation);

        lc_utility::BinaryFileRead bufferIn{fileBufferPath};

        arrSize = bufferIn.size() / sizeof(int16_t);
        intBuffer.resize(arrSize);
        bufferIn.read(intBuffer);
    }

    buffer.resize(arrSize);
    for (size_t i = 0; i < arrSize; i++)
        buffer[i] = lc_audio::int16ToSample<float>(intBuffer[i]);
}

void AdvancedAudioPlayer::custom_file::save(const std::string &fileDataPath, const std::string &fileBufferPath, unsigned int numChannels, unsigned int sampleRate, unsigned int bpm, double beatOffset, size_t playOffset, const std::vector<Keyframe> &automation, const std::vector<float> &buffer, bool bufferDataHasChanged)
{
    lc_utility::BinaryFileWrite dataOut{fileDataPath};

    dataOut.write((uint16_t)1); // latest file version

    dataOut.write((uint8_t)numChannels);
    dataOut.write((uint32_t)sampleRate);
    dataOut.write((uint32_t)bpm);
    dataOut.write(beatOffset);
    dataOut.write((uint64_t)playOffset);

    size_t automationArrSize = automation.size();
    dataOut.write((uint64_t)automationArrSize);
    dataOut.write(automation);

    if (bufferDataHasChanged)
    {
        size_t arrSize = buffer.size();
        std::vector<int16_t> intBuffer(arrSize);
        for (size_t i = 0; i < arrSize; i++)
            intBuffer[i] = lc_audio::sampleToInt16(buffer[i]);

        lc_utility::BinaryFileWrite bufferOut{fileBufferPath};
        bufferOut.write(intBuffer);
    }
}