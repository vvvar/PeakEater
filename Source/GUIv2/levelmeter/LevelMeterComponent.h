#pragma once

#include "../../DSP/LevelMeter.h"
#include <juce_events/juce_events.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <sound_meter/sound_meter.h>

namespace pe
{
namespace gui
{
class LevelMeterComponent : public juce::Component
{
public:
    LevelMeterComponent (std::shared_ptr<pe::dsp::LevelMeter<float>> levelMeter);
    ~LevelMeterComponent() override;

    void resized() override;

private:
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

    LevelMeterTimer mMeterTimer;
    sd::SoundMeter::MetersComponent mMeters;
    std::shared_ptr<pe::dsp::LevelMeter<float>> mLevelMeter;

    void onTimerTick();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LevelMeterComponent)
};
} // namespace gui
} // namespace pe