#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "../DSP/LevelMeter.h"
#include "Ticks.h"
#include "levelmeter/LevelMeterComponent.h"

namespace pe
{
namespace gui
{

class LeftPanel : public juce::Component
{
public:
LeftPanel (std::shared_ptr<pe::dsp::LevelMeter<float> > inputLevelMeter,
           std::shared_ptr<Ticks> ticks);

void resized() override;
void paint (juce::Graphics& g) override;

private:
LevelMeterComponent mLevelMeter;

JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LeftPanel)
};
} // namespace gui
} // namespace pe