#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "../DSP/LevelMeter.h"
#include "linkinout/LinkInOut.h"

namespace pe
{
namespace gui
{

class LinkingPanel : public juce::Component
{
public:
    LinkingPanel (std::shared_ptr<juce::AudioProcessorValueTreeState> parameters,
                  std::shared_ptr<pe::dsp::LevelMeter<float>> inputLevelMeter,
                  std::shared_ptr<pe::dsp::LevelMeter<float>> clippingLevelMeter,
                  std::shared_ptr<pe::dsp::LevelMeter<float>> outputLevelMeter);

    void resized() override;

private:
    LinkInOut mLinkInOut;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LinkingPanel)
};
} // namespace gui
} // namespace pe