#include "ControlPanel.h"

#include "ColourScheme.h"

namespace pe::gui
{
namespace
{
    int constexpr gBorderWith = 1;
    int constexpr gBorderRadius = 10;
} // namespace
ControlPanel::ControlPanel (std::shared_ptr<juce::AudioProcessorValueTreeState> parameters,
                            LevelMetersPack const& levelMetersPack)
    : juce::Component(), mDial_1 ("Input", parameters, "InputGain"), mDial_2 ("Type", parameters, "ClippingType"),
      mDial_3 ("Ceiling", parameters, "Ceiling", levelMetersPack.inputLevelMeter, levelMetersPack.clippingLevelMeter),
      mDial_4 ("Oversample", parameters, "OversampleRate"), mDial_5 ("Output", parameters, "OutputGain"),
      mDial_6 ("Dry/Wet", parameters, "DryWet")
{
    addAndMakeVisible (mDial_1);
    addAndMakeVisible (mDial_2);
    addAndMakeVisible (mDial_3);
    addAndMakeVisible (mDial_4);
    addAndMakeVisible (mDial_5);
    addAndMakeVisible (mDial_6);
}

void ControlPanel::paint (juce::Graphics& g)
{
    g.setColour (colourscheme::BackgroundSecondary.withAlpha (0.9f));
    auto bounds = getLocalBounds().toFloat().reduced (gBorderWith);
    g.fillRoundedRectangle (bounds, gBorderRadius);
    g.setColour (colourscheme::BackgroundTertiary.withAlpha (0.5f));
    g.drawRoundedRectangle (bounds, gBorderRadius, 1);
}

void ControlPanel::resized()
{
    juce::Grid grid;
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;
    using Item = juce::GridItem;
    grid.templateRows = { Track (Fr (1)) };
    grid.templateColumns = { Track (Fr (1)), Track (Fr (1)), Track (Fr (1)),
                             Track (Fr (1)), Track (Fr (1)), Track (Fr (1)) };
    grid.items = { Item (mDial_1), Item (mDial_2), Item (mDial_3), Item (mDial_4), Item (mDial_5), Item (mDial_6) };
    auto bounds = getLocalBounds();
    bounds.removeFromTop (5);
    bounds.removeFromLeft (5);
    bounds.removeFromRight (5);
    bounds.removeFromBottom (10);
    grid.performLayout (bounds);
}
} // namespace pe::gui
