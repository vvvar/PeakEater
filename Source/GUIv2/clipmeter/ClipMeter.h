#pragma once

#include <deque>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "../../DSP/LevelMeter.h"

namespace pe
{
namespace gui
{
class ClipMeter : public juce::Component
{
    class ClipMeterTimer : public juce::Timer
    {
    public:
        ClipMeterTimer (std::function<void()> callback)
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
    ClipMeter (std::shared_ptr<juce::AudioProcessorValueTreeState> parameters,
               std::shared_ptr<pe::dsp::LevelMeter<float>> inputLevelMeter,
               std::shared_ptr<pe::dsp::LevelMeter<float>> clippingLevelMeter,
               std::shared_ptr<pe::dsp::LevelMeter<float>> outputLevelMeter);
    ~ClipMeter() override;

    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    ClipMeterTimer mTimer;
    std::shared_ptr<juce::AudioProcessorValueTreeState> mParameters;
    std::shared_ptr<pe::dsp::LevelMeter<float>> mInputLevelMeter;
    std::shared_ptr<pe::dsp::LevelMeter<float>> mClippingLevelMeter;
    std::shared_ptr<pe::dsp::LevelMeter<float>> mOutputLevelMeter;
    std::deque<float> mInputBuffer;
    std::deque<float> mClippingBuffer;
    std::deque<float> mOutputBuffer;
    int mBufferMaxSize;
    std::vector<float> mTicks;

    void onTimerTick();

    void drawBuffer (std::deque<float>& buffer, juce::Colour& colour, juce::Graphics& g);
    void drawBuffer (std::deque<float>& buffer, juce::Colour&& colour, juce::Graphics& g);
    void drawDbLine (float const& dB, juce::Colour& colour, juce::Graphics& g);
    void drawLevels (std::shared_ptr<pe::dsp::LevelMeter<float>> inputMeter,
                     std::shared_ptr<pe::dsp::LevelMeter<float>> clippingMeter,
                     std::shared_ptr<pe::dsp::LevelMeter<float>> outputMeter,
                     juce::Graphics& g);
    void drawTicks (std::vector<float> const& ticksLevels, juce::Colour& colour, juce::Graphics& g);
    void drawTicks (std::vector<float> const& ticksLevels, juce::Colour&& colour, juce::Graphics& g);
    void drawTicksTexts (std::vector<float> const& ticksLevels, juce::Colour& colour, juce::Graphics& g);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ClipMeter)
};
} // namespace gui
} // namespace pe