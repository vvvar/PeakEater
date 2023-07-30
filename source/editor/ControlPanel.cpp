#include "ControlPanel.h"

#include "ColourScheme.h"

namespace pe::gui {
namespace {
int constexpr gBorderWith = 1;
int constexpr gBorderRadius = 10;
}  // namespace
ControlPanel::ControlPanel(std::shared_ptr<juce::AudioProcessorValueTreeState> parameters, LevelMetersPack const& levelMetersPack)
    : juce::Component(),
      inputGain("Input", parameters, "InputGain"),
      clippingType("Type", parameters, "ClippingType"),
      ceiling("Ceiling", parameters, "Ceiling", levelMetersPack.inputLevelMeter, levelMetersPack.clippingLevelMeter),
      oversample("Oversample", parameters, "OversampleRate"),
      dryWet("Dry/Wet", parameters, "DryWet"),
      outputGain("Output", parameters, "OutputGain") {
    addAndMakeVisible(inputGain);
    addAndMakeVisible(clippingType);
    addAndMakeVisible(ceiling);
    addAndMakeVisible(oversample);
    addAndMakeVisible(dryWet);
    addAndMakeVisible(outputGain);
}

void ControlPanel::paint(juce::Graphics& g) {
    g.setColour(colourscheme::BackgroundSecondary.withAlpha(0.9f));
    auto const bounds = getLocalBounds().toFloat().reduced(gBorderWith);
    g.fillRoundedRectangle(bounds, gBorderRadius);
    g.setColour(colourscheme::BackgroundTertiary.withAlpha(0.5f));
    g.drawRoundedRectangle(bounds, gBorderRadius, 1);
}

void ControlPanel::resized() {
    juce::Grid grid;
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;
    using Item = juce::GridItem;
    grid.templateRows = {Track(Fr(1))};
    grid.templateColumns = {Track(Fr(1)), Track(Fr(1)), Track(Fr(1)), Track(Fr(1)), Track(Fr(1)), Track(Fr(1))};
    grid.items = {Item(inputGain), Item(clippingType), Item(ceiling), Item(dryWet), Item(oversample), Item(outputGain)};
    auto bounds = getLocalBounds();
    bounds.removeFromTop(5);
    bounds.removeFromLeft(5);
    bounds.removeFromRight(5);
    bounds.removeFromBottom(10);
    grid.performLayout(bounds);
}
}  // namespace pe::gui
