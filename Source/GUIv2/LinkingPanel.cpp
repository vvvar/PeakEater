#include "LinkingPanel.h"

#include "ColourScheme.h"

namespace pe
{
namespace gui
{
LinkingPanel::LinkingPanel (std::shared_ptr<juce::AudioProcessorValueTreeState> parameters,
                            std::shared_ptr<pe::dsp::LevelMeter<float> > inputLevelMeter,
                            std::shared_ptr<pe::dsp::LevelMeter<float> > clippingLevelMeter,
                            std::shared_ptr<pe::dsp::LevelMeter<float> > outputLevelMeter)
	: juce::Component()
	, mLinkInOut (parameters)
{
	addAndMakeVisible (mLinkInOut);
}

void LinkingPanel::paint (juce::Graphics& g)
{
	// g.fillAll(colourscheme::BackgroundSecondary);
}

void LinkingPanel::resized()
{
	mLinkInOut.setBounds (getLocalBounds());
}
} // namespace gui
} // namespace pe