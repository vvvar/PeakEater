#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "../Ticks.h"

#include <atomic>

namespace pe
{
namespace gui
{
class ScalingSwitch : public juce::Component, public juce::TooltipClient
{
public:
ScalingSwitch (std::shared_ptr<Ticks> ticks);
~ScalingSwitch() override;

void paint (juce::Graphics& g) override;
void mouseDown (juce::MouseEvent const& event) override;
void mouseEnter (juce::MouseEvent const& event) override;
void mouseExit (juce::MouseEvent const& event) override;
juce::String getTooltip() override;

private:
std::shared_ptr<Ticks> mTicks;

JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ScalingSwitch)
};
} // namespace gui
} // namespace pe