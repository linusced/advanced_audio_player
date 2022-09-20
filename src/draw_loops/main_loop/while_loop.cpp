#include "draw_loops/main_loop.hpp"

void MainLoop::loop()
{
    double fpsPrevTime{0.0}, fpsInterval{0.1};
    int fpsFrameCount{0};
    bool resizeInit{true}, waveformInit{false};

    int prevProgress{0}, currentProgress{0};
    double prevProgressTime{0.0};

    while (!window->shouldClose())
    {
        window->update();
        const double &currentTime = window->getCurrentTime();

        if (window->hasResized() || resizeInit)
        {
            resizeInit = false;

            guiRenderer->resize();
            loadingGUI->resize();
            fileExplorer->resize();

            waveformScrollbarResize();
        }

        if (devMode)
        {
            if (currentTime - fpsPrevTime > fpsInterval)
            {
                int fps = fpsFrameCount / (currentTime - fpsPrevTime);
                window->setTitle(std::to_string(fps) + " fps");
                fpsPrevTime = currentTime;
                fpsFrameCount = 0;
            }
            else
                fpsFrameCount++;
        }

        audioPlayer->update();

        if (openFileExplorer)
        {
            fileExplorer->open(audioPlayer->getFilePath(0));
            openFileExplorer = false;
        }
        else if (resetLoading)
        {
            loadingProgressWidth->value = prevProgress = currentProgress = 0;
            prevProgressTime = 0.0;
            loadingProgress->styleChange = true;
            savingText->isVisible = pitchChangeText->isVisible = bounceText->isVisible = false;
            loadingText->isVisible = true;
            resetLoading = false;
        }
        else if (saveProject)
        {
            loadingProgressWidth->value = prevProgress = currentProgress = 0;
            prevProgressTime = 0.0;
            loadingProgress->styleChange = true;
            savingText->isVisible = true;
            loadingText->isVisible = pitchChangeText->isVisible = bounceText->isVisible = false;

            audioPlayer->saveFiles();
            saveProject = false;
        }
        else if (pitchChange)
        {
            loadingProgressWidth->value = prevProgress = currentProgress = 0;
            prevProgressTime = 0.0;
            loadingProgress->styleChange = true;
            pitchChangeText->isVisible = true;
            loadingText->isVisible = savingText->isVisible = bounceText->isVisible = false;

            audioPlayer->setFileBPM(waveformDataArr[pitchChangeIndex].pitchBPM, pitchChangeIndex);
            pitchChange = false;
        }
        else if (audioPlayer->fileThreadReset() && pitchChangeText->isVisible)
        {
            waveformElement->setData(&audioPlayer->getFile(pitchChangeIndex)->getFullBuffer(), audioPlayer->getFileAutomation(pitchChangeIndex), pitchChangeIndex);
            waveformElement->setBeats(audioPlayer->getFileBeats(pitchChangeIndex)->getBeatInterval(), audioPlayer->getFileBeats(pitchChangeIndex)->getBeatOffset(), pitchChangeIndex);
            waveformElement->setOffset(audioPlayer->getFilePlayOffset(pitchChangeIndex), pitchChangeIndex);
            waveformElement->drawWaveforms();

            std::string bpmStr = std::to_string(waveformDataArr[pitchChangeIndex].pitchBPM);
            waveformDataArr[pitchChangeIndex].bpmInput->setText(bpmStr);
            waveformDataArr[pitchChangeIndex].pitchChangeBpmInput->setText(bpmStr);
            waveformDataArr[pitchChangeIndex].beatOffsetInput->setText(std::to_string(audioPlayer->getFileBeats(pitchChangeIndex)->getBeatOffset()));
        }
        else if (bounceProject)
        {
            fileExplorer->open(audioPlayer->getFilePath(0), true, "bounce.wav");
            explorerType = FILE_EXPLORER_BOUNCE_PROJECT;
            bounceProject = false;
        }

        if (prevProgress != 100)
        {
            frameProgress(prevProgress, currentProgress, prevProgressTime, waveformInit, currentTime);
        }
        else if (fileExplorer->isActive())
        {
            fileExplorer->update();

            if (fileExplorer->userHasSelectedAFile())
            {
                std::string selectedFilePath = fileExplorer->getSelectedFilePath();

                switch (explorerType)
                {
                case FILE_EXPLORER_ADD_FILE:
                {
                    fileManagerItemsArr.push_back(FileManagerItem());
                    auto &item = fileManagerItemsArr.back();
                    item.waveformIndex = audioPlayer->getNumFiles();
                    item.filePath = selectedFilePath;

                    updateFileManager(false);
                }
                break;
                case FILE_EXPLORER_BOUNCE_PROJECT:
                {
                    loadingProgressWidth->value = prevProgress = currentProgress = 0;
                    prevProgressTime = 0.0;
                    loadingProgress->styleChange = true;
                    bounceText->isVisible = true;
                    loadingText->isVisible = savingText->isVisible = pitchChangeText->isVisible = false;

                    audioPlayer->bounceFiles(selectedFilePath);
                }
                break;
                }

                guiRenderer->update();
            }
        }
        else
        {
            if (displayKeyframEditor)
            {
                keyframeEditor->isVisible = true;
                displayKeyframEditor = false;
            }

            if (updateFileManagerNextFrame)
                updateFileManager(true);

            if (updateWaveformDataNextFrame)
                updateWaveformData();

            if (audioPlayer->isPlaying() && audioPlayer->getCurrentTime() > waveformElement->getMaxTime())
            {
                togglePlaybackBtn->click();
                audioPlayer->setCurrentTime(0.0);
            }

            waveformElement->setPlayHead(audioPlayer->getCurrentTime());

            guiRenderer->update();

            if (!guiRenderer->frameInputsActive())
                keyHandler.update();
        }

        glClear(GL_COLOR_BUFFER_BIT);

        if (waveformInit)
            guiRenderer->draw();

        if (prevProgress != 100)
            loadingGUI->draw();
        else if (fileExplorer->isActive())
            fileExplorer->draw();

        window->swapBuffers();
    }
}