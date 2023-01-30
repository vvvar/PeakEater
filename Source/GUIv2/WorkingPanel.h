#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "../DSP/LevelMeter.h"
#include "Ticks.h"
#include "CentralPanel.h"
#include "LeftPanel.h"
#include "RightPanel.h"

namespace pe
{
namespace gui
{

class WorkingPanel : public juce::Component
{
public:
WorkingPanel (std::shared_ptr<juce::AudioProcessorValueTreeState> parameters,
              std::shared_ptr<pe::dsp::LevelMeter<float> > inputLevelMeter,
              std::shared_ptr<pe::dsp::LevelMeter<float> > clippingLevelMeter,
              std::shared_ptr<pe::dsp::LevelMeter<float> > outputLevelMeter);

void resized() override;

private:
std::shared_ptr<Ticks> mTicks;
LeftPanel mLeftPanel;
CentralPanel mCentralPanel;
RightPanel mRightPanel;

JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WorkingPanel)
};
} // namespace gui
} // namespace pe