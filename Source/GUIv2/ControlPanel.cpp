#include "ControlPanel.h"

namespace pe
{
namespace gui
{
ControlPanel::ControlPanel (std::shared_ptr<juce::AudioProcessorValueTreeState> parameters,
                            std::shared_ptr<pe::dsp::LevelMeter<float> > inputLevelMeter,
                            std::shared_ptr<pe::dsp::LevelMeter<float> > clippingLevelMeter,
                            std::shared_ptr<pe::dsp::LevelMeter<float> > outputLevelMeter)
	: juce::Component()
	, mDial_1 ("Input", parameters, "InputGain")
	, mDial_2 ("Type", parameters, "ClippingType")
	, mDial_3 ("Ceiling", parameters, "Ceiling", inputLevelMeter, clippingLevelMeter)
	, mDial_4 ("Oversample", parameters, "OversampleRate")
	, mDial_5 ("Output", parameters, "OutputGain")
{
	addAndMakeVisible (mDial_1);
	addAndMakeVisible (mDial_2);
	addAndMakeVisible (mDial_3);
	addAndMakeVisible (mDial_4);
	addAndMakeVisible (mDial_5);
}

void ControlPanel::resized()
{
	juce::Grid grid;
	using Track = juce::Grid::TrackInfo;
	using Fr = juce::Grid::Fr;
	using Item = juce::GridItem;
	grid.templateRows = { Track (Fr (1)) };
	grid.templateColumns = { Track (Fr (1)), Track (Fr (1)), Track (Fr (1)), Track (Fr (1)), Track (Fr (1)) };
	grid.items = { Item (mDial_1), Item (mDial_2), Item (mDial_3), Item (mDial_4), Item (mDial_5) };
	grid.performLayout (getLocalBounds());
}
} // namespace gui
} // namespace pe