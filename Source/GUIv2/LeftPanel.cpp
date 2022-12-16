#include "LeftPanel.h"

namespace pe
{
namespace gui
{
LeftPanel::LeftPanel (std::shared_ptr<juce::AudioProcessorValueTreeState> parameters,
                      std::shared_ptr<pe::dsp::LevelMeter<float> > inputLevelMeter,
                      std::shared_ptr<pe::dsp::LevelMeter<float> > clippingLevelMeter,
                      std::shared_ptr<pe::dsp::LevelMeter<float> > outputLevelMeter)
	: juce::Component()
	, mLevelMeter (inputLevelMeter, "IN")
{
	addAndMakeVisible (mLevelMeter);
}

void LeftPanel::resized()
{
	mLevelMeter.setBounds (getLocalBounds());
}

void LeftPanel::paint (juce::Graphics& g)
{
	g.fillAll (juce::Colour (22, 33, 62));
}
} // namespace gui
} // namespace pe