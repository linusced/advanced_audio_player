#include "draw_loops/main_loop.hpp"

void MainLoop::frameProgress(int &prevProgress, int &currentProgress, double &prevProgressTime, bool &waveformInit, const double &currentTime)
{
    constexpr double PROGRESS_TRANSITION_DURATION{0.1};

    if (prevProgressTime == 0.0)
    {
        currentProgress = audioPlayer->getProgress() * 100;
        if (currentProgress != prevProgress)
            prevProgressTime = currentTime;
    }
    else if (currentTime - prevProgressTime < PROGRESS_TRANSITION_DURATION)
    {
        loadingProgressWidth->value = ((currentTime - prevProgressTime) / PROGRESS_TRANSITION_DURATION) * (currentProgress - prevProgress) + prevProgress;
        loadingProgress->styleChange = true;
    }
    else
    {
        prevProgressTime = 0.0;
        loadingProgressWidth->value = prevProgress = currentProgress;

        if (currentProgress == 100)
        {
            loadingProgressWidth->value = 0;
            togglePlaybackText->setText(audioPlayer->isPlaying() ? "Pause" : "Play");

            if (loadingText->isVisible)
            {
                if (updateFilesAfterLoading)
                {
                    updateFiles();
                    updateFilesAfterLoading = false;
                }
                else
                {
                    updateWaveformData();
                    updateFileManager(true);
                    waveformInit = true;
                }
            }
        }

        loadingProgress->styleChange = true;
    }

    loadingGUI->update();
}