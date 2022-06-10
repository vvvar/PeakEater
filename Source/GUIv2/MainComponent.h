#pragma once

#include <juce_events/juce_events.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <sound_meter/sound_meter.h>

#include "../PluginProcessor.h"

namespace pe
{
namespace gui
{

class MainComponent : public juce::Component
{
    class LevelMeterTimer : public juce::Timer
    {
    public:
        LevelMeterTimer (std::function<void()> callback)
            : mCallback (callback)
        {
        }

        void timerCallback() override
        {
            if (mCallback)
            {
                mCallback();
            }
        }

    private:
        std::function<void()> mCallback;
    };

public:
    MainComponent (std::shared_ptr<pe::dsp::LevelMeter<float>> levelMeter);
    void resized() override;

private:
    LevelMeterTimer mMeterTimer;
    sd::SoundMeter::MetersComponent mMeters;
    std::shared_ptr<pe::dsp::LevelMeter<float>> mLevelMeter;

    void onTimerTick();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
} // namespace gui
} // namespace pe