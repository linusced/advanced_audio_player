#pragma once

#include "lc_audio/lc_audio.hpp"
#include "lc_utility/lc_utility.hpp"

class AdvancedAudioPlayer : public lc_audio::Engine
{
public:
    struct Keyframe
    {
        uint64_t time;
        float amplitude, lowPassFrequency, highPassFrequency;

        Keyframe(uint64_t _time, float _amplitude = 1.0f, float _lowPassFrequency = 20000.0f, float _highPassFrequency = 20.0f)
            : time(_time), amplitude(_amplitude), lowPassFrequency(_lowPassFrequency), highPassFrequency(_highPassFrequency) {}
    };

    AdvancedAudioPlayer(std::string projectFolderPath);
    ~AdvancedAudioPlayer();

    const size_t BUFFER_SIZE_MONO, METRONOME_DURATION;
    const double D_NUM_CHANNELS, D_SAMPLE_RATE, BUFFER_SIZE_SECONDS, METRONOME_DURATION_SECONDS;

    void update() override;

    const bool &fileThreadReset() const;

    void setVolume(float volume) override;
    const float &getVolume() const override;

    void setMetronomeVolume(float metronomeVolume);
    const float &getMetronomeVolume() const;

    size_t getNumFiles() const;

    const double &getCurrentTime() const;
    void setCurrentTime(double time);

    lc_audio::File *getFile(size_t index);
    lc_audio::Beats *getFileBeats(size_t index);
    const std::string &getFileName(size_t index) const;
    const std::string &getFilePath(size_t index) const;

    const size_t &getFileDuration(size_t index) const;
    const size_t &getFilePlayOffset(size_t index) const;
    const double &getFileDuration_d(size_t index) const;
    const double &getFilePlayOffset_d(size_t index) const;
    std::vector<Keyframe> *getFileAutomation(size_t index);

    void setFilePlayOffset(size_t offset, size_t index);
    void setFilePlayOffset(double offset, size_t index);
    void setFileBPM(unsigned int bpm, size_t index);

    const bool &isMuted(size_t index) const;
    void mute(size_t index);
    void unmute(size_t index);

    const float &getProgress() const;

    void saveFiles();

    void swapFileLocations(size_t index1, size_t index2);
    void removeFiles(size_t startIndex, size_t numFiles);

    void loadFiles(std::vector<std::string> audioFilePaths);

    void bounceFiles(const std::string &filePath);

    static constexpr float MIN_FREQUENCY{20.0f}, MAX_FREQUENCY{20000.0f};
    static float exponentialFrequencyRange(float frequency);
    static float exponentialFrequencyRangeInverse(float range);

private:
    struct FileData
    {
        std::unique_ptr<lc_audio::File> file;
        std::unique_ptr<lc_audio::Beats> beats;
        std::string name, filePath;
        size_t duration, playOffset;
        std::vector<Keyframe> automation;
        std::vector<float> prevLowPassValues, prevHighPassValues;
        bool muted{false}, bufferDataHasChanged{false};
    };

    void updateBufferData() override;
    void advanced_updateBufferData(bool bounce);

    std::vector<FileData> files;

    std::string projectFolderPath;

    size_t bufferOffset{0}, bufferOffsetMono;
    double currentTime{0.0};

    float filesVolume{1.0f}, metronomeVolume{0.0f};
    std::unique_ptr<lc_audio::File> metronomeAudioData;

    std::mutex bufferDataMutex, fileLoadingMutex;

    bool fileLoadingComplete{false}, stopFileLoading{false}, initFilesLoaded{false}, fileSavingComplete{false}, filePitchChangeComplete{false}, fileBounceComplete{false},
        updateFileThreadReset{false};
    float progress{0.0f};

    std::unique_ptr<std::thread> fileThread;
    static void fileLoadingThread_func(AdvancedAudioPlayer *instance, std::vector<std::string> audioFilePaths);
    static void fileSavingThread_func(AdvancedAudioPlayer *instance);
    static void filePitchChangeThread_func(AdvancedAudioPlayer *instance, size_t index, double pitch);
    static void fileBounceThread_func(AdvancedAudioPlayer *instance, std::string filePath);

    static const std::string CUSTOM_FILE_DATA_EXTENSION, CUSTOM_FILE_BUFFER_EXTENSION;
    struct custom_file
    {
        static void load(const std::string &fileDataPath, const std::string &fileBufferPath, unsigned int &numChannels, unsigned int &sampleRate, unsigned int &bpm, double &beatOffset, size_t &playOffset, std::vector<Keyframe> &automation, std::vector<float> &buffer);
        static void save(const std::string &fileDataPath, const std::string &fileBufferPath, unsigned int numChannels, unsigned int sampleRate, unsigned int bpm, double beatOffset, size_t playOffset, const std::vector<Keyframe> &automation, const std::vector<float> &buffer, bool bufferDataHasChanged);
    };
};