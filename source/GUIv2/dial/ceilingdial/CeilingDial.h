#pragma once

#include "../../../DSP/LevelMeter.h"
#include "../Dial.h"
#include "CeilingDialLookAndFeel.h"

namespace pe
{
namespace gui
{
    class CeilingDial : public Dial
    {
    public:
        CeilingDial (std::string const& labelText,
                     std::shared_ptr<juce::AudioProcessorValueTreeState> parameters,
                     std::string const& parameterId,
                     std::shared_ptr<pe::dsp::LevelMeter<float>> inputLevelMeter,
                     std::shared_ptr<pe::dsp::LevelMeter<float>> clippingLevelMeter);
        ~CeilingDial();

    private:
        class ClipDialTimer : public juce::Timer
        {
        public:
            ClipDialTimer (std::function<void()> callback)
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

        CeilingDialLookAndFeel mCeilingDialLookAndFeel;
        ClipDialTimer mTimer;

        void onTimerTick();

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CeilingDial)
    };
} // namespace gui
} // namespace pe