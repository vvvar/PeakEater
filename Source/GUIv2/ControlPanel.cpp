#include "ControlPanel.h"

#include "ColourScheme.h"

namespace pe
{
namespace gui
{
namespace
{
int constexpr gBorderWith = 1;
int constexpr gBorderRadius = 10;
} // namespace
ControlPanel::ControlPanel (std::shared_ptr<juce::AudioProcessorValueTreeState> parameters,
                            std::shared_ptr<pe::dsp::LevelMeter<float> > inputLevelMeter,
                            std::shared_ptr<pe::dsp::LevelMeter<float> > clippingLevelMeter)
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

void ControlPanel::paint (juce::Graphics& g)
{
	g.setColour(colourscheme::BackgroundSecondary.withAlpha(0.9f));
	auto bounds = getLocalBounds().toFloat().reduced(gBorderWith);
	g.fillRoundedRectangle(bounds, gBorderRadius);
	g.setColour (colourscheme::BackgroundTertiary.withAlpha(0.5f));
	g.drawRoundedRectangle (bounds, gBorderRadius, 1);
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
	auto bounds = getLocalBounds();
	bounds.removeFromTop(5);
	bounds.removeFromLeft(5);
	bounds.removeFromRight(5);
	bounds.removeFromBottom(10);
	grid.performLayout (bounds);
}
} // namespace gui
} // namespace pe