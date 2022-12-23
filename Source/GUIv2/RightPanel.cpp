#include "RightPanel.h"

namespace pe
{
namespace gui
{
RightPanel::RightPanel (std::shared_ptr<juce::AudioProcessorValueTreeState> parameters,
                        std::shared_ptr<pe::dsp::LevelMeter<float> > inputLevelMeter,
                        std::shared_ptr<pe::dsp::LevelMeter<float> > clippingLevelMeter,
                        std::shared_ptr<pe::dsp::LevelMeter<float> > outputLevelMeter)
	: juce::Component()
	, mLevelMeter (outputLevelMeter, "OUT")
{
	addAndMakeVisible (mLevelMeter);
}

void RightPanel::resized()
{
	mLevelMeter.setBounds (getLocalBounds());
}
} // namespace gui
} // namespace pe