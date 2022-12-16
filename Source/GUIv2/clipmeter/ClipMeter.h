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

class Ticks
{
public:
std::vector<float>& getTicksList()
{
	return mTicksLists.at (mCurrentTicksList);
}

void switchToNextTicksList()
{
	if (mCurrentTicksList >= mTicksLists.size() - 1)
	{
		mCurrentTicksList = 0;
	}
	else
	{
		mCurrentTicksList++;
	}
}

private:
std::atomic<unsigned int> mCurrentTicksList{ 0 };
std::vector<std::vector<float> > mTicksLists{
	{ 0.0f, -3.0f, -5.0f, -8.0f, -11.0f, -14.0f, -17.0f, -20.0f, -23.0f, -26.0f, -29.0f, -32.0f, -36.0f },
	{ 0.0f, -3.0f, -6.0f, -9.0f, -12.0f, -15.0f, -18.0f, -21.0f, -24.0f, -27.0f, -30.0f, -33.0f, -36.0f },
	{ 0.0f, -5.0f, -8.0f, -11.0f, -14.0f, -17.0f, -20.0f, -23.0f, -26.0f, -29.0f, -32.0f, -36.0f },
	{ 0.0f, -4.0f, -6.0f, -8.0f, -12.0f, -14.0f, -16.0f, -18.0f, -20.0f, -22.0f, -24.0f, -26.0f, -28.0f, -30.0f, -32.0f, -34.0f, -36.0f },
};
};

public:
ClipMeter (std::shared_ptr<juce::AudioProcessorValueTreeState> parameters,
           std::shared_ptr<pe::dsp::LevelMeter<float> > inputLevelMeter,
           std::shared_ptr<pe::dsp::LevelMeter<float> > clippingLevelMeter,
           std::shared_ptr<pe::dsp::LevelMeter<float> > outputLevelMeter);
~ClipMeter() override;

void paint (juce::Graphics& g) override;
void resized() override;

private:
ClipMeterTimer mTimer;
std::shared_ptr<juce::AudioProcessorValueTreeState> mParameters;
std::shared_ptr<pe::dsp::LevelMeter<float> > mInputLevelMeter;
std::shared_ptr<pe::dsp::LevelMeter<float> > mClippingLevelMeter;
std::shared_ptr<pe::dsp::LevelMeter<float> > mOutputLevelMeter;
std::deque<float> mInputBuffer;
std::deque<float> mClippingBuffer;
std::deque<float> mOutputBuffer;
int mBufferMaxSize;
Ticks mTicks;

void onTimerTick();

void drawBuffer (std::deque<float>& buffer, juce::Colour& colour, juce::Graphics& g);
void drawBuffer (std::deque<float>& buffer, juce::Colour&& colour, juce::Graphics& g);
void drawDbLine (float const& dB, juce::Colour& colour, juce::Graphics& g);

void drawTicks (std::vector<float> const& ticksLevels, juce::Colour& colour, juce::Graphics& g);
void drawTicks (std::vector<float> const& ticksLevels, juce::Colour&& colour, juce::Graphics& g);
void drawTicksTexts (std::vector<float> const& ticksLevels, juce::Colour& colour, juce::Graphics& g);

void mouseDown (juce::MouseEvent const& event) override;

JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ClipMeter)
};
} // namespace gui
} // namespace pe