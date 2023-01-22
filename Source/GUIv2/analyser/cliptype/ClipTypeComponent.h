#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

namespace pe
{
namespace gui
{
class ClipTypeComponent
	: public juce::Component
{
public:
ClipTypeComponent (std::shared_ptr<juce::AudioProcessorValueTreeState> parameters);
~ClipTypeComponent() override;

void paint (juce::Graphics& g) override;
void mouseDown (juce::MouseEvent const& event) override;
std::vector<float>const& getCurrentWave() const;

private:
std::shared_ptr<juce::AudioProcessorValueTreeState> mParameters;
std::map<juce::String const, std::vector<float> const> mClippedWaveTables;

JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ClipTypeComponent)
};
} // namespace gui
} // namespace pe