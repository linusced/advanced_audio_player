#include "draw_loops/main_loop.hpp"

void MainLoop::createKeyHandler()
{
    auto lmbTypeChange = [this](int id)
    {
        waveformElement->setLmbType((audio_player_gui::Waveform::LMB_TYPE)id);
    };

    for (unsigned int i = 0; i <= (unsigned int)audio_player_gui::Waveform::LMB_CREATE_KEYFRAME; i++)
    {
        keyHandler.addKey({GLFW_KEY_1 + i}, false, lmbTypeChange, i);
        keyHandler.addKey({GLFW_KEY_KP_1 + i}, false, lmbTypeChange, i);
    }

    keyHandler.addKey({GLFW_KEY_SPACE}, false, [this](int)
                      { togglePlaybackBtn->click(); });

    keyHandler.addKey({GLFW_KEY_ESCAPE}, false, [this](int)
                      {if(keyframeEditor->isVisible) keyframeEditor->isVisible = false; });

    keyHandler.addKey({GLFW_KEY_S, GLFW_KEY_LEFT_CONTROL}, false, [this](int)
                      { saveProjectBtn->click(); });

    keyHandler.addKey({GLFW_KEY_S, GLFW_KEY_RIGHT_CONTROL}, false, [this](int)
                      { saveProjectBtn->click(); });

    keyHandler.addKey({GLFW_KEY_M}, false, [this](int)
                      {
            if (volumeSlider->getValue() != 0.0f)
            {
                prevVolumeValue = volumeSlider->getValue();
                volumeSlider->setValue(0.0f, true);
            }
            else
                volumeSlider->setValue(prevVolumeValue, true); });

    keyHandler.addKey({GLFW_KEY_K}, false, [this](int)
                      {
            if (metronomeSlider->getValue() != 0.0f)
            {
                prevMetronomeValue = metronomeSlider->getValue();
                metronomeSlider->setValue(0.0f, true);
            }
            else
                metronomeSlider->setValue(prevMetronomeValue, true); });

    keyHandler.addKey({GLFW_KEY_UP}, true, [this](int)
                      { auto *slider = volumeSlider.get();
                        if (glfwGetKey(*window, GLFW_KEY_LEFT_SHIFT) || glfwGetKey(*window, GLFW_KEY_RIGHT_SHIFT))
                            slider = metronomeSlider.get();

                        slider->setValue(slider->getValue() + 0.1f, true); });

    keyHandler.addKey({GLFW_KEY_DOWN}, true, [this](int)
                      { auto *slider = volumeSlider.get();
                        if (glfwGetKey(*window, GLFW_KEY_LEFT_SHIFT) || glfwGetKey(*window, GLFW_KEY_RIGHT_SHIFT))
                            slider = metronomeSlider.get();

                        slider->setValue(slider->getValue() - 0.1f, true); });

    keyHandler.addKey({GLFW_KEY_B, GLFW_KEY_LEFT_CONTROL}, false, [this](int)
                      { bounceProjectBtn->click(); });

    keyHandler.addKey({GLFW_KEY_B, GLFW_KEY_RIGHT_CONTROL}, false, [this](int)
                      { bounceProjectBtn->click(); });
}