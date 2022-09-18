#pragma once

#include <algorithm>
#include <atomic>
#include <deque>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <numeric>

#include "../../DSP/LevelMeter.h"

namespace pe
{
namespace gui
{
class AnalyserComponent
    : public juce::Component
{
public:
    AnalyserComponent (std::shared_ptr<juce::AudioProcessorValueTreeState> parameters,
                       std::shared_ptr<pe::dsp::LevelMeter<float>> inputLevelMeter,
                       std::shared_ptr<pe::dsp::LevelMeter<float>> clippingLevelMeter,
                       std::shared_ptr<pe::dsp::LevelMeter<float>> outputLevelMeter);
    ~AnalyserComponent() override;

    void resized() override;
    void paint (juce::Graphics& g) override;
    void drawLevels (float inputLevel,
                     float outputLevel,
                     float eatenAmount,
                     juce::Graphics& g);

    void mouseDown (juce::MouseEvent const& event) override;

private:
    class AnalyserTimer : public juce::Timer
    {
    public:
        AnalyserTimer (std::function<void()> callback)
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

    class PeakAnalyzer
    {
    public:
        PeakAnalyzer (int const& sampleRate, float const& minValue)
            : mMinValue (minValue)
            , mBufferSize (sampleRate * 2) // 2 seconds of look behind
        {
            reset();
        }

        void push (float const& nextValue)
        {
            mBuffer.pop_front();
            mBuffer.push_back (nextValue);
        }

        float getMagnitude()
        {
            return *std::max_element (mBuffer.begin(), mBuffer.end());
        }

        void reset()
        {
            mBuffer.clear();
            for (int x = 0; x < mBufferSize; x++)
            {
                mBuffer.push_back (mMinValue);
            }
        }

    private:
        float mMinValue;
        int const mBufferSize;
        std::deque<float> mBuffer;
    };

    std::shared_ptr<pe::dsp::LevelMeter<float>> mInputLevelMeter;
    std::shared_ptr<pe::dsp::LevelMeter<float>> mClippingLevelMeter;
    std::shared_ptr<pe::dsp::LevelMeter<float>> mOutputLevelMeter;

    PeakAnalyzer mInputAnalyzer;
    PeakAnalyzer mOutputAnalyzer;
    PeakAnalyzer mEatenAnalyzer;
    AnalyserTimer mAnalyzerUpdateTimer;

    void onAnalyzerUpdateTick();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AnalyserComponent)
};
} // namespace gui
} // namespace pe